import torch
import cv2
#squeeze_channel
class SqueezeChannel:
    def __init__(self, channel = 0):
        self.channel = channel

    def __call__(self, x):
        return x.squeeze(self.channel)

#to_tensor_long
class ToTensorLong:
    def __call__(self, x):
        return torch.tensor(x, dtype=torch.long)
    
#to_tensor_float
class ToTensorFloat:
    def __call__(self, x):
        return torch.tensor(x, dtype=torch.float)
    
class Resize:
    def __init__(self, size, interpolation=cv2.INTER_LINEAR):
        self.size = size
        self.interpolation = interpolation

    def __call__(self, x):
        return cv2.resize(x, self.size, interpolation=self.interpolation)