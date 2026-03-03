import os

import numpy as np
from imgui.integrations.glfw import GlfwRenderer
import OpenGL.GL as gl
import glfw
from torch.utils.data import DataLoader

import sys
import cv2
import torch
from segmentation_dataset import *
from torchvision import transforms
import torch.nn as nn
import torch.optim as optim
from tqdm import tqdm

import src.networks.unet_2d as unet_2d
from matplotlib import pyplot as plt

from src.utils.data_generator import StudyInfo

x = torch.rand(5, 3)
print(x)
print(torch.cuda.is_available())


def prepare_study_info(root, arterydata_file):

    done_file_path = root + arterydata_file
    with open(done_file_path, "r") as f:
        lines = f.readlines()
        i = 0

        volumes = ["", ""]
        keywords = ["DATA_0:", "DATA_1:"]
        ki = 0
        while i < len(lines):
            if lines[i].strip() == keywords[ki]:
                volumes[ki] = lines[i+1].split("/")[1].strip()
                if ki == 0:
                    data_dir = lines[i + 1].split("/")[0] + "/"
                ki += 1
                i += 1
            i += 1

            if ki == len(keywords):
                break

        return StudyInfo(root, data_dir, *volumes)


def main():
    data_dir = "D:/Praca/JadraKostnienia/jadrakostnienia2024/"
    jobX_path = data_dir + "jobX.job3"

    studies = []

    with open(jobX_path, "r") as f:
        for l in f:
            parts = l.split("/")
            data_root = data_dir + parts[-2] + "/"
            info = prepare_study_info(data_root, parts[-1].strip())
            if not info.has_surfaces:
                print(f"Dataset {info.root} has no surfaces. Skipping...")
                continue
            studies.append(info)

    mid = int(len(studies) * 0.8)
    train_studies = studies[: mid]
    test_studies = studies[mid:]

    transform = transforms.Compose([
        transforms.Resize((256, 256)),
        transforms.ToTensor()
    ])

    train_segmentation_dataset = SegmentationDataset(train_studies, transform)
    test_segmentation_dataset = SegmentationDataset(test_studies, transform)

    train_loader = DataLoader(train_segmentation_dataset)
    test_loader = DataLoader(test_segmentation_dataset)

    device = "cuda" if torch.cuda.is_available() else "cpu"

    model = unet_2d.UNet_2D(in_channels=1, out_channels=1)

    cont_checkpoint_name = "models/best_model.pth"
    cont_checkpoint_name = "checkpoints/latest_checkpoint.pth"
    cont_checkpoint_name = "checkpoints/checkpoint_e37.pth"

    if os.path.exists(cont_checkpoint_name):
        checkpoint = torch.load(cont_checkpoint_name)
        if 'model_state_dict' in checkpoint:
            model.load_state_dict(checkpoint['model_state_dict'])
        else:
            model.load_state_dict(checkpoint)
    else:
        print(f"Cannot find file: {cont_checkpoint_name}. Exitting...")
        return


    model.to(device)
    model.eval()

    img, mask = train_segmentation_dataset.get_slice_data(0, 64)

    with torch.no_grad():
        # for images, masks in test_loader:
        image = img.unsqueeze(0).to(device)

        output = model(image)

        pred_mask = torch.sigmoid(output)  # zamiana logits -> prawdopodobieństwo
        pred_mask = (pred_mask > 0.5).float()  # binary mask

        plt.figure(figsize=(10, 4))

        plt.subplot(1, 3, 1)
        plt.title("Oryginalny obraz")
        plt.imshow(img[0])

        plt.subplot(1, 3, 2)
        plt.title("Maska oryginalna")
        plt.imshow(mask[0])

        plt.subplot(1, 3, 3)
        plt.title("Maska segmentacji")
        plt.imshow(pred_mask.squeeze().cpu())

        plt.show()


if __name__ == '__main__':
    main()
