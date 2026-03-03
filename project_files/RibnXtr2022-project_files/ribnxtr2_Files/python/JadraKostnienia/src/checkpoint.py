
import datetime
import os
from pathlib import Path
import torch

class LossValues:
    def __init__(self, ckpt_train_loss=float('inf'), ckpt_val_loss=float('inf'), best_val_loss=float('inf')):
        self.ckpt_train_loss = ckpt_train_loss
        self.ckpt_val_loss = ckpt_val_loss
        self.best_val_loss = best_val_loss

    def save_to_dict(self):
        return {
            "ckpt_train_loss": self.ckpt_train_loss,
            "ckpt_val_loss": self.ckpt_val_loss,
            "best_val_loss": self.best_val_loss,
        }

    def load_from_dict(self, data):
        self.ckpt_train_loss = data.get("ckpt_train_loss", -1)
        self.ckpt_val_loss = data.get("ckpt_val_loss", -1)
        self.best_val_loss = data.get("best_val_loss", -1)


class CheckpointData:
    def __init__(self, name="", epoch=-1, model=None, optimizer=None, loss_values=LossValues(), config={}):
        if len(name) == 0:
            current_timestamp = datetime.datetime.now()
            formatted_timestamp = current_timestamp.strftime("%Y_%m_%d_%H_%M")

            self.experiment_name = f"experiment_{formatted_timestamp}"
        else:
            self.experiment_name = name

        self.epoch = epoch

        self.model_state = {}
        if model is not None:
            self.model_state = model.state_dict()

        self.optimizer_state = {}
        if optimizer is not None:
            self.optimizer_state = optimizer.state_dict()
        self.loss_values = loss_values

        self.config = config
        # self.best_validation_loss = best_validation_loss
        # self.checkpoint_valalidation_loss = checkpoint_valalidation_loss

    def load_from_dict(self, checkpoint_dict):
        self.experiment_name = checkpoint_dict.get("experiment_name", "")
        self.epoch = checkpoint_dict.get("epoch", -1)
        self.model_state = checkpoint_dict.get("model_state", {})
        self.optimizer_state = checkpoint_dict.get("optimizer_state", {})
        self.config = checkpoint_dict.get("config", {})
        self.loss_values = LossValues()
        lv = checkpoint_dict.get("loss_values", None)
        if lv is not None:
            self.loss_values.load_from_dict(lv)
        

        # self.best_validation_loss = checkpoint_dict.get("best_validation_loss", -1)
        # self.checkpoint_valalidation_loss = checkpoint_dict.get("checkpoint_valalidation_loss", -1)

    def save_to_dict(self):
        return {
            "experiment_name": self.experiment_name,
            "epoch": self.epoch,
            "model_state": self.model_state,
            "optimizer_state": self.optimizer_state,
            "loss_values": self.loss_values.save_to_dict(),
            "config": self.config
            # "best_validation_loss": self.best_validation_loss,
            # "checkpoint_valalidation_loss": self.checkpoint_valalidation_loss,
        }


def load_checkpoint(path: Path):
    checkpoint = CheckpointData()
    if path.exists():
        data = torch.load(path)
        checkpoint.load_from_dict(checkpoint_dict=data)
    return checkpoint

