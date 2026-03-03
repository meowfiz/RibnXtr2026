from loss.dice_loss import DiceLoss
from torch import nn

#cfg - yaml cfg
def build_loss(cfg):
    loss_type = cfg["type"]

    if loss_type == 'dice':
        return DiceLoss(
            cfg['eps']
        )
    
    elif loss_type == 'cross_entropy_loss':
        return nn.CrossEntropyLoss()

    raise ValueError(f"Unknown loss: {loss_type}")