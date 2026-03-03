import os

import numpy as np
from torch.utils.data import Dataset
from PIL import Image
import cv2

class SegmentationDataset(Dataset):
    def __init__(self, images_path, masks_path, transform):
        self.img_path = images_path
        self.mask_path = masks_path
        self.img_count = len(os.listdir(images_path))
        self.transform = transform

    def __len__(self):
        return self.img_count

    def __getitem__(self, idx):
        img = Image.open("{}\\{}.tif".format(self.img_path, idx))
        img_data = np.array(img)
        img_data = (img_data * 255).astype(np.uint8)
        img = Image.fromarray(img_data)
        img = self.transform(img)
        if self.mask_path is not None:
            # mask = Image.open(f"{self.mask_path}\\{idx}.png")
            mask = cv2.imread(f"{self.mask_path}\\{idx}.png", cv2.IMREAD_GRAYSCALE)
            mask = cv2.resize(mask, (256, 256), interpolation=cv2.INTER_NEAREST)
        else:
            # mask = Image.new("L", (10, 10), 0)
            mask = np.zeros(shape=(256, 256), dtype=np.uint8)
        # mask = self.transform(mask)

        return img, mask