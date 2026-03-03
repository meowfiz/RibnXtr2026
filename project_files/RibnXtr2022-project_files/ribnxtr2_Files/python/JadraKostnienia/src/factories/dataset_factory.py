from pathlib import Path
import numpy as np
import torch
from dataset.labeled_dataset_2D import LabeledDataset2D
from factories.transform_factory import build_transform
# dataroot: directory with output of volume_to_numpy.py script

def split_dataset(data, parts):
    p = np.array(parts)
    p = p / p.sum()
    n = len(data)
    np.random.shuffle(data)
    parts = [round(x * n) for x in np.cumsum(p)]
    parts = parts[:-1]
    return np.split(data, parts)


def build_dataset(cfg, data_root):
    ds_type = cfg['type']
    
    if ds_type == 'labeled_2d':

        data_transform_cfg = cfg.get('data_transform', None)
        mask_transform_cfg = cfg.get('mask_transform', None)

        labeled_data_path=data_root / cfg['data_path']

        if data_transform_cfg is None or mask_transform_cfg is None:
            raise ValueError(f"Dataset {ds_type} requires valid data_transform {data_transform_cfg} and mask_transform {mask_transform_cfg}")

        if not labeled_data_path.is_dir():
            raise ValueError(f"Dataset data path: {labeled_data_path} does not exists")

        data_transform = build_transform(data_transform_cfg)
        mask_transform = build_transform(mask_transform_cfg)

        filenames = [x.name for x in Path(labeled_data_path / 'volume').iterdir()]
        train_files, val_files = split_dataset(filenames, cfg['split'])

        train_dataset = LabeledDataset2D(
            labeled_data_path=labeled_data_path, 
            samples_per_epoch=cfg.get('train_samples_per_epoch', 10000),
            data_transform=data_transform,
            mask_transform=mask_transform,
            filenames = train_files
        )

        validation_dataset = LabeledDataset2D (
            labeled_data_path=labeled_data_path, 
            samples_per_epoch=cfg.get('val_samples_per_epoch', 2000),
            data_transform=data_transform,
            mask_transform=mask_transform,
            filenames = val_files
        )

        return (train_dataset, validation_dataset)

    raise ValueError(f"Unknown dataset: {ds_type}")