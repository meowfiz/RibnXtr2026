import argparse
from pathlib import Path

import numpy as np

from utils.rdata_to_numpy import read_rdata
from checkpoint import load_checkpoint
from factories.model_factory import build_model
from utils.config_utils import load_config
from utils.volume import Volume

import torch
import cv2


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--rdata_path", required=True)
    parser.add_argument("--config", required=True)
    parser.add_argument("--checkpoint", required=True)
    parser.add_argument("--output_mask", required=True)
    args = parser.parse_args()

    rdata_path = Path(args.rdata_path)
    header_path = Path(str(rdata_path) + ".header")
    if not rdata_path.exists():
        raise FileNotFoundError(f"Volume: {rdata_path} does not exist")
    if not header_path.exists():
        raise FileNotFoundError(f"Header: {header_path} does not exist")

    # 1) .rdata -> numpy volume
    volume_np = read_rdata(rdata_path, header_path)

    # zapisz tymczasowe .npy obok .rdata
    npy_path = rdata_path.with_suffix(".npy")
    np.save(npy_path, volume_np)

    # 2) segmentacja (logika jak w segmentation.py, ale bez UI)
    vol_path = npy_path
    conf_path = Path(args.config)
    ckpt_path = Path(args.checkpoint)
    output_path = Path(args.output_mask)

    if not conf_path.exists():
        raise FileNotFoundError(f"Config: {conf_path} does not exist")
    if not ckpt_path.exists():
        raise FileNotFoundError(f"Checkpoint: {ckpt_path} does not exist")

    device = "cuda" if torch.cuda.is_available() else "cpu"

    config = load_config(conf_path)
    checkpoint = load_checkpoint(ckpt_path)
    volume = Volume(vol_path)

    model = build_model(config["model"])
    model.load_state_dict(checkpoint.model_state)
    model.to(device)
    model.eval()

    result_volume = np.zeros(shape=volume.shape, dtype=np.uint8)

    with torch.no_grad():
        for i in range(volume.shape[0]):
            slice_ = volume.get_normalized_slice(i)
            resized_slice = cv2.resize(slice_, (256, 256), interpolation=cv2.INTER_NEAREST)
            img = torch.from_numpy(resized_slice).unsqueeze(0).unsqueeze(0).to(device)

            categories = model(img)
            mask = torch.argmax(categories, 1)
            result = np.array(mask[0].cpu(), dtype=np.uint8)
            result = cv2.resize(result, result_volume[0].shape[::-1], interpolation=cv2.INTER_NEAREST)

            result_volume[i] = result

    # 3) zapis maski w formacie, który wczyta RibnXtr
    # Zakładam: surowy uint8 3D tak jak inne .mask
    result_volume.tofile(output_path)


if __name__ == "__main__":
    main()