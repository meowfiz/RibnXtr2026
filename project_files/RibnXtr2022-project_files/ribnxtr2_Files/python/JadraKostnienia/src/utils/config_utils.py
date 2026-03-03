import yaml
from pathlib import Path

# paths = {
#     "base": Path("configs").resolve(),
#     "loss": (Path("configs") / "loss").resolve(),
#     "model": (Path("configs") / "model").resolve(),
#     "optimizer": (Path("configs") / "optimizer").resolve(),
#     "transform": (Path("configs") / "transform").resolve(),
#     "dataset":  (Path("configs") / "dataset").resolve(),
# }
keywords = ["base", "loss", "model", "optimizer", "transform", "dataset"]

available_yamls = {x.name : x for x in Path("configs").resolve().rglob('*.yaml')}

def resolve_yamls(d):
    if isinstance(d, list):
        for x in d:
            resolve_yamls(x)
    
    elif isinstance(d, dict):
        for k, v in d.items():
            if isinstance(v, str) and v.endswith(".yaml"):
                d[k] = load_config(available_yamls[v])
            else:
                resolve_yamls(d[k])


def load_config(yaml_path):
    if not yaml_path.exists():
        raise ValueError(f"Path {yaml_path} does not exist")    

    with open(yaml_path) as f:
        cfg = yaml.safe_load(f)

    #if config have root, remove it:
    if len(cfg) == 1:
        key = list(cfg.keys())[0]
        if key in keywords:
            cfg = cfg[key]

    resolve_yamls(cfg)
        
    # for k, v in cfg.items():
    #     if k in paths and isinstance(v, str) and v.endswith(".yaml"):
    #         sub_yaml_path = paths[k] / v
    #         cfg[k] = load_config(sub_yaml_path)[k]

    return cfg

def save_config(output_yaml_path, data_dict):
    s = yaml.safe_dump(data_dict)
    with open(output_yaml_path, 'w') as f:
        f.write(s)