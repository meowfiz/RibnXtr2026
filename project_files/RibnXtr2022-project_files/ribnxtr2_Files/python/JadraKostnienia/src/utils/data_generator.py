import os

import numpy as np
from PIL import Image
import cv2


class StudyInfo:
    def __init__(self, root, study_directory, volume_1, volume_2):
        self.surfaces = "voxelizedSurfaces.rdata"
        self.root = root
        self.data_dir = study_directory
        self.volume_1 = volume_1
        self.volume_2 = volume_2
        self.has_surfaces = os.path.exists(root + study_directory + self.surfaces)

        with open(root + study_directory + volume_1 + ".header", "r") as f:
            lines = f.readlines()
            self.size = [int(x) for x in lines[1].split()]
            self.size.reverse() #d h w

    def get_volume_1_path(self):
        return self.root + self.data_dir + self.volume_1

    def get_volume_2_path(self):
        return self.root + self.data_dir + self.volume_2

    def get_surfaces_path(self):
        if not self.has_surfaces:
            print(f"[WARNING] dataset {self.root} has no sufraces")
        return self.root + self.data_dir + self.surfaces


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

    studies_with_surfaces = []
    studies_without_surfaces = []

    # prepare study data (paths, masks, volume etc)
    with open(jobX_path, "r") as f:
        for path in f:
            parts = path.split("/")
            data_root = data_dir + parts[-2] + "/"
            info = prepare_study_info(data_root, parts[-1].strip())
            if not info.has_surfaces:
                print(f"Dataset {info.root} has no surfaces. Skipping...")
                studies_without_surfaces.append(info)
            else:
                studies_with_surfaces.append(info)

    # generate TEST (dane bez masek, do "wzrokowego" testowania sieci)
    if len(os.listdir("data/test/")) == 0:
        counter = 0
        for study in studies_without_surfaces:
            loaded_volume = np.fromfile(study.get_volume_1_path(), dtype='<u2')
            loaded_volume = np.reshape(loaded_volume, study.size)

            for slice in range(study.size[0]):
                image = Image.fromarray(loaded_volume[slice] / loaded_volume.max())
                image.save(f"data/test/{counter}.tif")
                counter += 1

    # generate IMAGES and MASKS
    # if len(os.listdir("data/images/")) == 0:
    #     counter = 0
    #     for study in studies_with_surfaces:
    #         loaded_volume = np.fromfile(study.get_volume_1_path(), dtype='<u2')
    #         loaded_volume = np.reshape(loaded_volume, study.size)

    #         loaded_surfaces = np.fromfile(study.get_surfaces_path(), dtype='<u2')
    #         loaded_surfaces = np.reshape(loaded_surfaces, study.size)

    #         for slice in range(study.size[0]):
    #             image = Image.fromarray(loaded_volume[slice] / loaded_volume.max())
    #             image.save(f"data/images/{counter}.tif")
    #             mask = Image.fromarray(loaded_surfaces[slice])
    #             mask.save(f"data/masks/{counter}.png")
    #             counter += 1
    
    counter = 0
    for study in studies_with_surfaces:
        # loaded_volume = np.fromfile(study.get_volume_1_path(), dtype='<u2')
        # loaded_volume = np.reshape(loaded_volume, study.size)

        loaded_surfaces = np.fromfile(study.get_surfaces_path(), dtype='<u2')
        loaded_surfaces = np.reshape(loaded_surfaces, study.size)

        for slice in range(study.size[0]):
            # image = Image.fromarray(loaded_volume[slice] / loaded_volume.max())
            # image.save(f"data/images/{counter}.tif")
            mask = Image.fromarray(loaded_surfaces[slice].astype(np.uint8))
            mask.save(f"data/masks/{counter}.png")
            counter += 1

    # im = Image.open("data/images/img_1022.tif")
    # print(im.mode)
    # img = np.array(im)
    # cv2.imshow("test", img)
    # cv2.waitKey(0)


if __name__ == '__main__':
    main()