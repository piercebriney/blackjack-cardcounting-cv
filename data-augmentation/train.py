from ultralytics import YOLO

model = YOLO('yolov8l.pt')

results = model.train(data='yolo_config.yml', epochs=50, imgsz=1280, device=0, batch=-1, patience=10)