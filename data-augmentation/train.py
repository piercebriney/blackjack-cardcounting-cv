from ultralytics import YOLO

model = YOLO('runs/detect/train2/weights/last.pt')

results = model.train(data='yolo_config.yml', epochs=50, imgsz=1280, device=0, batch=-1, patience=10, resume=True)