import numpy as np
from torch.utils.data import Dataset
from pathlib import Path
from utils.volume import Volume
from PIL import Image
import cv2

class LabeledDataset2D(Dataset):

    def get_datasets(self, volume_dir, mask_dir):
        result = []
        for path in Path(volume_dir).iterdir():
            f = path.name
            result.append((volume_dir / f, mask_dir / f if mask_dir else None ))
        return result

    def __init__(self, labeled_data_path, samples_per_epoch, data_transform, mask_transform, filenames):
        self.data = []

        for f in filenames:
            self.data.append((labeled_data_path / "volume" / f, labeled_data_path / "mask" / f))

        self.samples_per_epoch = samples_per_epoch
        self.data_transform = data_transform
        self.mask_transform = mask_transform

        self.augmentations = []
        
        # zakladam, ze augmentation to pewne obiekty / zestaw flag, ktore mowia jak getitem ma pobierac dane. Len -> ile danych wyprodukować

        # self.img_path = images_path
        # self.mask_path = masks_path
        # self.img_count = len(os.listdir(images_path))
        # self.transform = transform

    def __len__(self):
        return self.samples_per_epoch

    def __getitem__(self, idx):
        volume_id = np.random.randint(0, len(self.data))
        dataset = self.data[volume_id]

        data_vol = Volume(dataset[0])
        mask_vol = Volume(dataset[1])
        slice = np.random.randint(0, data_vol.data.shape[0])
        img = data_vol.get_normalized_slice(slice)
        # img = Image.fromarray((img * 255).astype(np.uint8))
        img = self.data_transform(img)

        mask = mask_vol.get_slice(slice)
        # mask = Image.fromarray(mask.astype(np.uint8))
        mask = self.mask_transform(mask)

        return img, mask
