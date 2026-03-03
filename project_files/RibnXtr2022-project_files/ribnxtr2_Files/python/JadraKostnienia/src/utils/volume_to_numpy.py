import os

import numpy as np
from PIL import Image
import cv2
from pathlib import Path
"""
Script converts marked data (JadraKostnienia/JadraKostnienia2024) to numpy volumes and masks
"""

class StudyInfo:
    def __init__(self, root, study_directory, volume_1, volume_2):
        self.surfaces = "voxelizedSurfaces.rdata"
        self.root = root
        self.data_dir = study_directory
        self.volume_1 = volume_1
        self.volume_2 = volume_2
        self.has_surfaces = os.path.exists(root / study_directory / self.surfaces)

        with open(root / study_directory / f"{volume_1}.header", "r") as f:
            lines = f.readlines()
            self.size = [int(x) for x in lines[1].split()]
            self.size.reverse() #d h w

    def get_volume_1_path(self):
        return self.root / self.data_dir / self.volume_1

    def get_volume_2_path(self):
        return self.root / self.data_dir / self.volume_2

    def get_surfaces_path(self):
        if not self.has_surfaces:
            print(f"[WARNING] dataset {self.root} has no sufraces")
        return self.root / self.data_dir / self.surfaces


def prepare_study_info(root, arterydata_file):
    done_file_path = root / arterydata_file
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
    data_root = Path("D:/Praca/JadraKostnienia/")
    data_dir = data_root / "jadrakostnienia2024/"
    jobX_path = data_dir / "jobX.job3"

    unlabeled_dir = data_root / "unlabeled"
    labeled_dir_volume = data_root / "labeled" / "volume"
    labeled_dir_mask = data_root / "labeled" / "mask"

    studies_with_surfaces = []
    studies_without_surfaces = []

    with open(jobX_path, "r") as f:
        for path in f:
            parts = path.split("/")
            root = data_dir / parts[-2]
            info = prepare_study_info(root, parts[-1].strip())
            if not info.has_surfaces:
                print(f"Dataset {info.root} has no surfaces. Skipping...")
                studies_without_surfaces.append(info)
            else:
                studies_with_surfaces.append(info)

    os.makedirs(unlabeled_dir, exist_ok=True)
    
    for study in studies_without_surfaces:
        loaded_volume = np.fromfile(study.get_volume_1_path(), dtype='<u2')
        loaded_volume = np.reshape(loaded_volume, study.size)
        np.save(unlabeled_dir / study.root.name, loaded_volume)

    os.makedirs(labeled_dir_volume, exist_ok=True)
    os.makedirs(labeled_dir_mask, exist_ok=True)
    for study in studies_with_surfaces:
        loaded_volume = np.fromfile(study.get_volume_1_path(), dtype='<u2')
        loaded_volume = np.reshape(loaded_volume, study.size)

        loaded_surfaces = np.fromfile(study.get_surfaces_path(), dtype='<u2')
        loaded_surfaces = np.reshape(loaded_surfaces, study.size)

        np.save(labeled_dir_volume / study.root.name, loaded_volume)
        np.save(labeled_dir_mask / study.root.name, loaded_surfaces)


if __name__ == '__main__':
    main()

    # test ile klas jest w danych

    # data_root = Path("D:/Praca/JadraKostnienia/")
    # data_dir = data_root / "jadrakostnienia2024/"
    # jobX_path = data_dir / "jobX.job3"

    # unlabeled_dir = data_root / "unlabeled"
    # labeled_dir_volume = data_root / "labeled" / "volume"
    # labeled_dir_mask = data_root / "labeled" / "mask"

    # classes = np.empty(shape=(0), dtype=np.uint16)

    # for x in labeled_dir_mask.iterdir():
    #     v = np.load(x)
    #     s = np.unique(v)
    #     classes = np.concatenate((classes, s))
    #     classes = np.unique(classes)
    #     print(v.shape, v.dtype, classes)
    #     print(s)