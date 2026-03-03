import numpy as np

class Volume:
    def __init__(self, volume_path):
        self.path = volume_path
        self.data = np.load(volume_path, mmap_mode="r")
        self.shape = self.data.shape

    def get_slice(self, slice_id):
        return np.array(self.data[slice_id])
    
    def get_normalized_slice(self, slice_id):
        return (np.array(self.data[slice_id]) / self.data.max()).astype(np.float32)

    def get_slice_w_neighbours(self, slice_id, r):
        pass

    def get_block(self, p, shape):
        pass