
```
python3 -m venv .venv-yoloe
source .venv-yoloe/bin/activate
pip install -U pip
pip install -U "ultralytics[export]" onnx onnxruntime onnxslim
python convert_to_det.py
```