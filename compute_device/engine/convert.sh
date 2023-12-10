#!/bin/sh
base=$(echo $1 | rev | cut -f 2- -d '.' | rev)
python3 export-det.py --weights $base.pt --iou-thres 0.65 --conf-thres 0.25 --topk 100 --opset 11 --sim --input-shape 1 3 1280 1280 --device cuda:0
python3 build.py --weights $base.onnx --iou-thres 0.65 --conf-thres 0.25 --topk 100 --fp16 --device cuda:0
