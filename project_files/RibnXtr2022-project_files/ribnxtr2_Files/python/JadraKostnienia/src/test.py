import datetime

import torch
from torchvision import transforms

from dataset.slice_2d_dataset import Slice2dDataset

from factories.dataset_factory import *
from utils.config_utils import *
from pathlib import Path

if __name__ == '__main__':

    masks = np.load("D:/maski.npy")
    print(masks.shape)
    print(masks.max())
    print(masks.dtype)

    volume = np.load("D:/test.npy")
    print(volume.shape)
    print(volume.max())
    print(volume.dtype)
    
    print(len({}))
    exit()
    # a = 0.15 + 0.15
    # b = 0.1 + 0.2
    # print(a == b)

    # current_timestamp = datetime.datetime.now()
    # formatted_timestamp = current_timestamp.strftime("%Y_%m_%d_%H_%M")
    # print("Formatted Timestamp:", formatted_timestamp)

    # transform = transforms.Compose([
    #     transforms.Resize((256, 256)),
    #     transforms.ToTensor()
    # ])

    # # ds = dataset.SegmentationDataset("data/images", "data/masks", transform)

    # ds = dataset.SegmentationDataset("data\\images", "data\\masks", transform)

    # train_segmentation_dataset, test_segmentation_dataset = torch.utils.data.random_split(ds, [0.8, 0.2])

    # a, b = ds.__getitem__(4776)

    # print("abc")
    # print(Path.cwd())
    d = load_config(Path('experiments')/'experiment_001'/'config.yaml')
    print(d)

    # save_config(Path('experiments')/'experiment_001'/'config_abcd.yaml', d)
    train_dataset, validation_dataset = build_dataset(d["dataset"], Path("D:/") / "Praca" / "JadraKostnienia")
    # data_root = Path("D:/Praca/JadraKostnienia/")

    # unlabeled_dir = data_root / "unlabeled"
    # labeled_dir_volume = data_root / "labeled" / "volume"
    # labeled_dir_mask = data_root / "labeled" / "mask"

    # Slice2dDataset(None, unlabeled_dir)

    # d = {'a': 'bcd'}
    # xd = d.copy()
    # print(d['a'])

    # def f(d):
    #     d['a'] = 'xD'

    # f(d)

    # print(d['a'])
    # print(xd)


    # d = {'a' : 'f'}

    # print(type(list(d.keys())))

