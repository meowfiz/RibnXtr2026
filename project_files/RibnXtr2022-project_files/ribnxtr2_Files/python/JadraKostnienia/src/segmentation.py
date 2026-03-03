
import argparse
from pathlib import Path

import cv2
import numpy as np
import torch

from checkpoint import load_checkpoint
from factories.model_factory import build_model
from utils.config_utils import load_config
from utils.volume import Volume


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("--volume", "-v", metavar="FILE.npy", help="Input volume data file (.npy).", required=True)
    parser.add_argument("--config", "-c", metavar="CONFIG.yaml", help="Model configuration file (.yaml).", required=True)
    parser.add_argument("--checkpoint", "-C", metavar="FILE.pth", help="Model checkpoint file (.pth).", required=True)
    parser.add_argument("--output", "-o", metavar="FILE.npy", help="Output volume segmentation file (.npy).", required=True)

    args = parser.parse_args()

    vol_path = Path(args.volume)
    conf_path = Path(args.config)
    ckpt_path = Path(args.checkpoint)
    output_path = Path(args.output)

    if not vol_path.exists():
        raise f"Volume: {vol_path} does not exist"
    
    if not conf_path.exists():
        raise f"Config: {conf_path} does not exist"
    
    if not ckpt_path.exists():
        raise f"Volume: {ckpt_path} does not exist"

    device = "cuda" if torch.cuda.is_available() else "cpu"

    config = load_config(conf_path)  
    checkpoint = load_checkpoint(ckpt_path)
    volume = Volume(vol_path)

    model = build_model(config['model'])
    model.load_state_dict(checkpoint.model_state)

    model.to(device)
    model.eval()

    result_volume = np.zeros(shape=volume.shape, dtype=np.uint8)

    with torch.no_grad():
        for i in range(volume.shape[0]):
            slice = volume.get_normalized_slice(i)
            resized_slice = cv2.resize(slice, (256, 256), interpolation=cv2.INTER_NEAREST)
            #model requires dim==4 (Batch, Categories, Height, Width), unsqueeze adds new 1 dim
            img = torch.from_numpy(resized_slice).unsqueeze(0).unsqueeze(0).to(device) 

            categories = model(img) # 16 categories / pixel (shape 1x16x256x256)
            mask = torch.argmax(categories, 1) # select best category (dim==1)
            result = np.array(mask[0].cpu(), dtype=np.uint8) #reduce to 2D
            #opencv uses shape(W, H) but numpy (H, W)
            result = cv2.resize(result, result_volume[0].shape[::-1], interpolation=cv2.INTER_NEAREST) #resize to original size
            
            result_volume[i] = result

            cv2.waitKey(100)
            cv2.imshow("DISPLAY", result * 16)
            print(i)


    np.save(output_path, result_volume)
    result_volume.tofile(f"{output_path}.bin")

if __name__ == '__main__':
    main()

# D:\Dev\PyCharmProjects\JadraKostnienia\src\segmentation.py -v "D:\test.npy" -c ".\experiments\experiment_001\resolved_config.yaml" -C .\experiments\experiment_001\checkpoints\best_checkpoint.pth -o "D:\maski.npy"