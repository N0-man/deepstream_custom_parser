from ultralytics import YOLOE
from ultralytics.utils.patches import torch_load

# Build a detection model skeleton, then load seg weights into it
det_model = YOLOE("yoloe-11l.yaml")
state = torch_load("yoloe-11l-seg.pt")
det_model.load(state["model"])
det_model.save("yoloe-11l-det.pt")

print("Saved: yoloe-11l-det.pt")