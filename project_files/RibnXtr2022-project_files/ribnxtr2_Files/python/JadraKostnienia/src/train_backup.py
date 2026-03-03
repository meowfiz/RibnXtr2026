from pathlib import Path
from torch.utils.data import DataLoader

import torch
from segmentation_dataset import *
from torchvision import transforms
import torch.nn as nn
import torch.optim as optim
from tqdm import tqdm
from factories import *
from src.models import *
import argparse
from utils.config_utils import *

from checkpoint import *

print("Cuda: ", torch.cuda.is_available())

def apply_checkpoint(checkpoint: CheckpointData, model, optimizer = None, device = "cuda"):
    model.load_state_dict(checkpoint.model_state)
    model.to(device)
    if optimizer is not None:
        optimizer.load_state_dict(checkpoint.optimizer_state)

# from https://cvinvolution.medium.com/dice-loss-in-medical-image-segmentation-d0e476eb486
# chat stwierdzil ze moja funkcja jest slaba, bo nie jest ciagla (count_nonzero zamiast sum).
# class DiceLoss(nn.Module):
#     def __init__(self):
#         super(DiceLoss, self).__init__()

#     def forward(self, pred, truth):
#         pred_area = torch.count_nonzero(pred)
#         truth_area = torch.count_nonzero(truth)
#         AoO = torch.count_nonzero(pred * truth)  # Area of Overlap

#         dice_coefficient = AoO * 2 / (pred_area + truth_area)

#         return 1 - dice_coefficient



def main():

    # process args
    parser = argparse.ArgumentParser()

    group = parser.add_mutually_exclusive_group(required=True)

    group.add_argument("--new", action="store_true",
                    help="Train from scratch. Requires config.yaml in experiment directory.")

    group.add_argument("--resume_best", action="store_true",
                    help="Resume from best checkpoint.")

    group.add_argument("--resume_last", action="store_true",
                    help="Resume from last checkpoint.")

    group.add_argument("--resume", type=str, metavar="FILE",
                    help="Resume from specified checkpoint file.")
    
    parser.add_argument("--directory", "-d", metavar="DIR", help="Experiment directory.", required=True)

    args = parser.parse_args()

    print(args.new,"  |  ", args.resume_best,"  |  ", args.resume_last,"  |  ", args.resume)

    experiment_root = Path(parser.directory)
    if not experiment_root.exists():
        print(f"Experiment directory {experiment_root} does not exist")
        return

    loaded_checkpoint = CheckpointData()
    file_to_load = ""
    
    if args.resume_best:
        file_to_load = experiment_root / "checkpoints" / "best_checkpoint.pth"
        
    elif args.resume_last:
        file_to_load = experiment_root / "checkpoints" / "last_checkpoint.pth"
        
    elif args.resume:
        file_to_load = args.resume

    else: #--new
        resolved_cfg_path = experiment_root / "resolved_config.yaml"
        if resolved_cfg_path.exists():
            cfg = load_config(resolved_cfg_path)    
        else:
            cfg = load_config(experiment_root / "config.yaml")
            save_config(resolved_cfg_path, cfg)

        loaded_checkpoint.config = cfg

    continue_learning = False

    if len(file_to_load) > 0:
        loaded_checkpoint = load_checkpoint(file_to_load)
        if loaded_checkpoint.epoch == -1:
            print(f"Cannot load {file_to_load}")
            return
        continue_learning = True

    #setup 
    transform = transforms.Compose([
        transforms.Resize((256, 256)),
        transforms.ToTensor()
    ])

    img_path = Path("data") / "images"
    mask_path = Path("data") / "masks"
    segmentation_dataset = SegmentationDataset(str(img_path), str(mask_path), transform)

    train_dataset, validation_dataset = torch.utils.data.random_split(segmentation_dataset, [0.8, 0.2])

    train_loader = DataLoader(train_dataset, batch_size=8, shuffle=True)
    validation_loader = DataLoader(validation_dataset)

    device = "cuda" if torch.cuda.is_available() else "cpu"
    
    model = build_model(load_checkpoint.config['model'])
    loss_function = build_loss(load_checkpoint.config['loss'])
    optimizer = build_optimizer(load_checkpoint.config['optimizer'])

    num_epochs = loaded_checkpoint.config['epochs']

    best_val_loss = loaded_checkpoint.loss_values.best_val_loss #float(inf) if new training

    if continue_learning:
        apply_checkpoint(loaded_checkpoint, model, optimizer, device)
        start_epoch = loaded_checkpoint.epoch + 1
        print(f"Resuming from checkpoint {file_to_load}. Start epoch: {start_epoch}, previous loss: {loaded_checkpoint.loss_values.ckpt_val_loss:.4f}")

    else:
        start_epoch = 1
        print(f"Starting new learning")

    model.to(device) 

    for epoch in range(start_epoch, num_epochs + 1):
        model.train()
        train_loss = 0.0

        print(f"\nEpoch {epoch}/{num_epochs}")

        for images, masks in tqdm(train_loader, desc="Training", leave=False):
            images = images.to(device)
            masks = masks.to(device)

            outputs = model(images)
            loss = loss_function(outputs, masks)  

            optimizer.zero_grad() 
            loss.backward() 
            optimizer.step()

            train_loss += loss.item()

        model.eval()

        print(f"Testing...")
        val_loss = 0.0
        with torch.no_grad():
            for images, masks in validation_loader:
                images = images.to(device)
                masks = masks.to(device)

                outputs = model(images)
                loss = loss_function(outputs, masks)
                val_loss += loss.item()

        avg_train_loss = train_loss / len(train_loader)
        avg_val_loss = val_loss / len(validation_loader)

        print(
            f"Epoch {epoch}/{num_epochs}, Train Loss: {avg_train_loss:.4f}, Val Loss: {avg_val_loss:.4f}")

        print(f"Saving checkpoints...")

        new_best = False
        if avg_val_loss < best_val_loss:
            best_val_loss = avg_val_loss
            new_best = True

        losses = LossValues(avg_train_loss, avg_val_loss, best_val_loss)
        current_checkpoint = CheckpointData(loaded_checkpoint.experiment_name, epoch, model, optimizer, losses)
        checkpoint = current_checkpoint.save_to_dict()
          
        torch.save(checkpoint, f"checkpoints/last_checkpoint.pth")

        if new_best:
            torch.save(checkpoint, "checkpoints/best_checkpoint.pth")
            print("✓ Saved new best checkpoint")

if __name__ == '__main__':
    main()
