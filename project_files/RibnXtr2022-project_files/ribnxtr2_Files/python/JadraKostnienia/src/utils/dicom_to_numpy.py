import io
import math
from pathlib import Path
import pydicom
import pydicom.fileset
import numpy as np
from rdata_to_numpy import read_rdata
import cv2
from matplotlib import pyplot as plt
import imgui.core as ig

from imgui_setup import ImguiWindow
import imgui

import math_utils

def read_dicomdir(dicom_dir_path):
    patients = []
    current_patient = {}
    current_study = {}
    current_series = {}

    ds = pydicom.dcmread(dicom_dir_path / 'DICOMDIR')
    for item in ds.DirectoryRecordSequence:
        type = item.DirectoryRecordType
        if type == "PATIENT":
            patients.append({
                'record': item,
                'studies': []
            })
            current_patient = patients[-1]
        elif type == "STUDY":
            current_patient['studies'].append({
                'record': item,
                'series': []
            })
            current_study = current_patient['studies'][-1]
        elif type == "SERIES":
            current_study['series'].append({
                'record': item,
                'images': []
            })
            current_series = current_study['series'][-1]
        elif type == "IMAGE":
            current_series['images'].append(item)
        else:
            raise ValueError(f"Unexpected DICOM record type {type}")
    return patients


def dicom_to_numpy(dicom_dir_path):
    if not dicom_dir_path.exists():
        raise ValueError(f"Directory {dicom_dir_path} does not exist")
    ds = pydicom.dcmread(dicom_dir_path / 'DICOMDIR')
    for item in ds.DirectoryRecordSequence[0: 10]:
        print(item.DirectoryRecordType)


def crop_volume(vol, morph_kernel, offset):
    norm_vol = math_utils.normalize_ndarray_uchar8(vol)

    # second return val is bins
    hist, _ = np.histogram(norm_vol, bins=256,range=(0, 256))

    total_pixels = vol.size
    limit = total_pixels / 8
    current_pixel_count = total_pixels

    i = 0
    while current_pixel_count > limit:
        current_pixel_count -= hist[i]
        i+=1

    thresh = i

    vol_mask = (norm_vol >= thresh).astype(np.uint8)*255
    
    kernel = morph_kernel
    for i in range(vol_mask.shape[0]):
        vol_mask[i] = cv2.erode(vol_mask[i], kernel)
        vol_mask[i] = cv2.dilate(vol_mask[i], kernel)

    zarr, yarr, xarr = vol_mask.nonzero()
    
    z_min = math_utils.clamp(zarr.min() - offset, 0, vol_mask.shape[0])
    z_max = math_utils.clamp(zarr.max() + offset + 1, 0, vol_mask.shape[0])

    y_min = math_utils.clamp(yarr.min() - offset, 0, vol_mask.shape[1])
    y_max = math_utils.clamp(yarr.max() + offset + 1, 0, vol_mask.shape[1])

    x_min = math_utils.clamp(xarr.min() - offset, 0, vol_mask.shape[2])
    x_max = math_utils.clamp(xarr.max() + offset + 1, 0, vol_mask.shape[2])

    cropped_volume = vol[z_min:z_max, y_min:y_max, x_min:x_max]
    
    return cropped_volume, (z_min, y_min, x_min)

    # seq = ds.get("DirectoryRecordSequence")
    # for x in seq:
        # if x.DirectoryRecordType == "PATIENT":
            # print(x)
    # print(root)
    # fs = pydicom.fileset.FileSet(ds)
    # # print(fs)
    # for instance in fs:
    #     print(instance.path)


if __name__ == '__main__':
    dicom_dir_path = Path("D:/Praca/JadraKostnienia/dicom/001-275")
    arr = read_dicomdir(dicom_dir_path)
    patient = arr[0]
    study = patient['studies'][0]
    series = study['series'][4]
    paths = [dicom_dir_path.joinpath(*img.ReferencedFileID) for img in series['images']]
    dcm_data = [pydicom.dcmread(p) for p in paths]
    frames = [d.pixel_array for d in dcm_data]
    volume = np.stack(frames)

    cropped_volume, offset = crop_volume(volume, np.ones((15, 15)), 10)
    print(offset)

    rdata_path = Path(r"D:\Praca\JadraKostnienia\jadrakostnienia2024\001-275\done\orig-bones - Cropped.rdata")
    header_path = Path(r"D:\Praca\JadraKostnienia\jadrakostnienia2024\001-275\done\orig-bones - Cropped.rdata.header")
    
    rdata_offset = (66, 141, 160) #recznie przepisalem z pliku .header

    rdata_volume = read_rdata(rdata_path, header_path)

    cropped_volume = (cropped_volume - cropped_volume.min()) / (cropped_volume.max() - cropped_volume.min())
    window = ImguiWindow()
    current = 0

    # slices = vol_float.shape[0]
    slices = cropped_volume.shape[0]

    while(not window.should_close()):
        window.begin_frame()    
        cv2.imshow("TEST", cropped_volume[current])

        _, current = imgui.slider_int("slice", current, min_value=0, max_value=slices - 1)
        # _, thresh = imgui.slider_int("thresh", thresh, min_value=0, max_value=255)
        
        imgui.text("Hello world!")
        window.end_frame()
        plt.close()

    window.cleanup()