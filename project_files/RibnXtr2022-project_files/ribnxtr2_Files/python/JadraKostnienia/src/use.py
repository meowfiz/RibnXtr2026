import os
from pathlib import Path

import numpy as np
from imgui.integrations.glfw import GlfwRenderer
import OpenGL.GL as gl
import glfw
from torch.utils.data import DataLoader

import sys
import cv2
import torch
from factories import *
from segmentation_dataset import *
from checkpoint import *
from torchvision import transforms
import torch.nn as nn
import torch.optim as optim
from tqdm import tqdm

from matplotlib import pyplot as plt

from utils.config_utils import load_config

x = torch.rand(5, 3)
print(x)
print(torch.cuda.is_available())

def apply_LUT(mask, lut_array):
    lut_r = lut_array[:, 0]
    lut_g = lut_array[:, 1]
    lut_b = lut_array[:, 2]

    mask_r = cv2.LUT(mask, lut_r)
    mask_g = cv2.LUT(mask, lut_g)
    mask_b = cv2.LUT(mask, lut_b)

    return cv2.merge((mask_r, mask_g, mask_b))

def main():
    transform = transforms.Compose([
        transforms.Resize((256, 256)),
        transforms.ToTensor()
    ])

    # experiment_root = Path(args.directory)
    config_path = Path("D:/Dev/PyCharmProjects/JadraKostnienia/experiments/experiment_001/resolved_config.yaml")
    checkpoint_path = Path("D:/Dev/PyCharmProjects/JadraKostnienia/experiments/experiment_001/checkpoints/best_checkpoint.pth")
    if not checkpoint_path.exists():
        print(f"Checkpoint path: {checkpoint_path} does not exists. Exiting...")
        exit(0)

    # data_path = Path("D:/") / "Praca" / "JadraKostnienia"

    # test_segmentation_dataset = SegmentationDataset(test_studies, None, transform)

    loaded_checkpoint = load_checkpoint(checkpoint_path)
    config = load_config(config_path)

    test_segmentation_dataset = SegmentationDataset("data/images", "data/masks", transform)

    test_loader = DataLoader(test_segmentation_dataset, batch_size=8)

    device = "cuda" if torch.cuda.is_available() else "cpu"

    model = build_model(config['model'])
    model.load_state_dict(loaded_checkpoint.model_state)

    # checkpoint_name = "checkpoints/best_checkpoint_2_100.pth"

    model.to(device)
    model.eval()
    i = 0

    
    class_color_lut = np.full((256, 3), fill_value=(255,0,255), dtype=np.uint8)
    class_color_lut[0]  = (0, 0, 0      ) # tlo
    class_color_lut[1]  = (157, 157, 157)
    class_color_lut[2]  = (255, 255, 255)
    class_color_lut[3]  = (190, 38, 51  )
    class_color_lut[4]  = (224, 111, 139)
    class_color_lut[5]  = (73, 60, 43   )
    class_color_lut[6]  = (164, 100, 34 )
    class_color_lut[7]  = (235, 137, 49 )
    class_color_lut[8]  = (247, 226, 107)
    class_color_lut[9]  = (47, 72, 78   )
    class_color_lut[10] = (68, 137, 26  )
    class_color_lut[11] = (163, 206, 39 )
    class_color_lut[12] = (27, 38, 50   )
    class_color_lut[13] = (0, 87, 132   )
    class_color_lut[14] = (49, 162, 242 )
    class_color_lut[15] = (178, 220, 239)

    with torch.no_grad():
        for images, masks in tqdm(test_loader, desc="Testing", leave=False):
            device_images = images.to(device)

            # outputs = model(images)
            # for images, masks in test_loader:
            # image = img.unsqueeze(0).to(device)
            output = model(device_images)

            # pred_mask = torch.sigmoid(output)  # zamiana logits -> prawdopodobieństwo
            # pred_mask = (pred_mask > 0.5).float()  # binary mask

            pred_mask = torch.argmax(output, 1)

            batch_size = images.shape[0]
            for x in range(batch_size):
                # print(output[x,:,0,0])
                # print("--")
                plt.figure(figsize=(10, 4))

                plt.subplot(1, 3, 1)
                plt.title("Oryginalny obraz")
                plt.imshow(images[x, 0].cpu())
                mask_np = np.array(masks[x].cpu(), dtype=np.uint8)
                mask_colored = apply_LUT(mask_np, class_color_lut)

                plt.subplot(1, 3, 2)
                plt.title("Maska oryginalna")
                plt.imshow(mask_colored)

                pred_mask_np = np.array(pred_mask[x].cpu(), dtype=np.uint8)
                pred_mask_colored = apply_LUT(pred_mask_np, class_color_lut)

                plt.subplot(1, 3, 3)
                plt.title("Maska segmentacji")
                plt.imshow(pred_mask_colored)
                i += 1
                plt.savefig(f"output/{i}.png")
            plt.close('all')


if __name__ == '__main__':
    main()
