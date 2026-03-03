import os

import numpy as np
from torch.utils.data import Dataset
import torch
from PIL import Image
from pathlib import Path

class Slice2dDataset(Dataset):

    def get_datasets(self, volume_dir, mask_dir):
        result = []
        for path in Path(volume_dir).iterdir():
            f = path.name
            result.append((volume_dir / f, mask_dir / f if mask_dir else None ))
        return result

    def __init__(self, labeled_data_path, unlabeled_data_path, augmentations, samples_per_epoch, seed):
        self.data = []
        if labeled_data_path:
            self.data.extend(self.get_datasets(labeled_data_path / "volume", labeled_data_path / "mask"))
        if unlabeled_data_path:
            self.data.extend(self.get_datasets(unlabeled_data_path, None))
        
        self.samples_per_epoch = samples_per_epoch
        
        # zakladam, ze augmentation to pewne obiekty / zestaw flag, ktore mowia jak getitem ma pobierac dane. Len -> ile danych wyprodukować

        # self.img_path = images_path
        # self.mask_path = masks_path
        # self.img_count = len(os.listdir(images_path))
        # self.transform = transform

    def __len__(self):
        return self.samples_per_epoch

    def __getitem__(self, idx):
        volume_id = np.random.randint(0, len(self.data), 1)
        dataset = self.data[volume_id]

        data_vol = np.load(dataset[0])
        
        # mask_vol = 
        for slice in range():
            image = Image.fromarray(loaded_volume[slice] / loaded_volume.max())
        img = Image.open("{}\\{}.tif".format(self.img_path, idx))
        img_data = np.array(img)
        img_data = (img_data * 255).astype(np.uint8)
        img = Image.fromarray(img_data)
        img = self.transform(img)
        if self.mask_path is not None:
            mask = Image.open("{}\\{}.png".format(self.mask_path, idx)).convert("L")
        else:
            mask = Image.new("L", (10, 10), 0)
        mask = self.transform(mask)

        return img, mask