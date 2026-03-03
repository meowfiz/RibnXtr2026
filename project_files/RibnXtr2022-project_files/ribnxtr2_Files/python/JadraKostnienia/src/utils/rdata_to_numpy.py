
import argparse
from pathlib import Path

import numpy as np

def read_rdata(rdata_path, header_path):
    with open(header_path, "r") as f:
        lines = f.readlines()
        volume_size = [int(x) for x in lines[1].split()]
        volume_size.reverse() #d h w

    loaded_volume = np.fromfile(rdata_path, dtype='<u2')
    loaded_volume = np.reshape(loaded_volume, volume_size)

    return loaded_volume

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("--volume", "-v", metavar="FILE.rdata", help="Input volume data file (.rdata).", required=True)
    parser.add_argument("--header", "-H", metavar="FILE.rdata.header", help="Volume header file. If omitted, volume.header will be used.")
    parser.add_argument("--output", "-o", metavar="FILE", help="Output Numpy volume.", required=True)
    
    args = parser.parse_args()

    vol_path = Path(args.volume)
    out_path = Path(args.output)

    if not vol_path.exists():
        raise FileNotFoundError(f"Volume: {vol_path} does not exist")

    if args.header == None or args.header == "":
        args.header = f"{args.volume}.header"

    header_path = Path(args.header)

    if not header_path.exists():
        raise FileNotFoundError(f"Header: {header_path} does not exist")

    loaded_volume = read_rdata(vol_path, header_path)
    np.save(out_path, loaded_volume)

if __name__ == '__main__':
    main()

# d:/Dev/PyCharmProjects/JadraKostnienia/src/rdata_to_numpy.py -v "D:\Praca\JadraKostnienia\jadrakostnienia2024\001-275\done\orig-bones - Cropped.rdata" -o "D:\test.npy"