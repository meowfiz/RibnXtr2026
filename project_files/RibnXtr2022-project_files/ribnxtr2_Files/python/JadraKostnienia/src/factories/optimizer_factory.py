import torch.optim as optim

def build_optimizer(cfg, model):
    opt_type = cfg["type"]
    if opt_type == 'adam':
        return optim.Adam(model.parameters(), lr=float(cfg['lr']))
    
    raise ValueError(f"Unknown optimizer: {opt_type}")