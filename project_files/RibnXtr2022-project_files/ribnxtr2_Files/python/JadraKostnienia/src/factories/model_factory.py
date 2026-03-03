from models.unet_2d import UNet_2D

#cfg - yaml cfg
def build_model(cfg):
    model_type = cfg["type"]

    if model_type == 'unet_2d':
        return UNet_2D(
            cfg['in_channels'],
            cfg['out_channels']
        )
    
    raise ValueError(f"Unknown model: {model_type}")