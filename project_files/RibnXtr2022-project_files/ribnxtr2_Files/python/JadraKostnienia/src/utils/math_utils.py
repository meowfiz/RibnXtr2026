import numpy as np

def clamp(val, lo, hi):
    return max(lo, min(val, hi))

def normalize_ndarray_float(data):
    result = data - data.min()
    return result / result.max()

def normalize_ndarray_uchar8(data):
    return (normalize_ndarray_float(data) * 255).astype(np.uint8)