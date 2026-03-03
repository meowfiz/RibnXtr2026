import torch
from torch import nn

class DiceLoss(nn.Module):
    def __init__(self, eps=1e-6):
        super().__init__()
        self.eps = eps

    def forward(self, pred, target):
        pred = torch.sigmoid(pred)

        pred = pred.view(-1)
        target = target.float().view(-1)

        intersection = (pred * target).sum()
        union = pred.sum() + target.sum()

        dice = (2 * intersection + self.eps) / (union + self.eps)

        return 1 - dice