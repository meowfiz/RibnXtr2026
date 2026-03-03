import numpy as np
from torch.utils.data import Dataset
from pathlib import Path
from utils.volume import Volume

class AutoencoderDataset2D(Dataset):

    def get_datasets(self, volume_dir):
        result = []
        for path in Path(volume_dir).iterdir():
            f = path.name
            result.append(volume_dir / f)
        return result

    def __init__(self, labeled_data_path, unlabeled_data_path, samples_per_epoch, seed, data_transform):
        self.data = []
        if labeled_data_path:
            self.data.extend(self.get_datasets(labeled_data_path / "volume"))
        if unlabeled_data_path:
            self.data.extend(self.get_datasets(unlabeled_data_path))
        
        self.samples_per_epoch = samples_per_epoch
        self.data_transform = data_transform
        

    def __len__(self):
        return self.samples_per_epoch

    def __getitem__(self, idx):
        volume_id = np.random.randint(0, len(self.data))
        dataset = self.data[volume_id]

        data_vol = Volume(dataset[0])

        slice = np.random.randint(0, data_vol.data.shape[0])
        img = data_vol.get_slice(slice)
        img = self.data_transform(img)


        return img