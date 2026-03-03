from torchvision import transforms
from transform.custom_transforms import *
import cv2

def get_resize(args):
    params = {}
    if not 'size' in args:
        raise ValueError(f"Resize 'size' parameter is required")
    
    params['size'] = tuple(args['size'])
    if 'interpolation' in args:
        if args['interpolation'] == 'nearest':
            params['interpolation'] = cv2.INTER_NEAREST
        elif args['interpolation'] == 'bilinear':
            params['interpolation'] = cv2.INTER_LINEAR
        elif args['interpolation'] == 'bicubic':
            params['interpolation'] = cv2.INTER_CUBIC
        else:
            raise ValueError(f"Invalid interpolation: {params['interpolation']}")

    return Resize(**params)

def get_transform(t_type, args):
    
    if t_type == 'resize':
        return get_resize(args)
    elif t_type == 'to_tensor':
        return transforms.ToTensor()
    elif t_type == 'to_tensor_float':
        return ToTensorFloat()
    elif t_type == 'to_tensor_long':
        return ToTensorLong()
    elif t_type == 'squeeze_channel':
        return SqueezeChannel(args.get('channel', 0))
    else:
        raise ValueError(f"Unknown transform type: {t_type}")

def build_transform(cfg):
    components = []
    for param in cfg:
        for k, v in param.items():
            components.append(get_transform(k, v))

    return transforms.Compose(components)
    