#call this first
#libcamera-vid -n -t 0 --width 1280 --height 960 --framerate 1 --inline --listen -o tcp://127.0.0.1:8888

from ultralytics import YOLO

model = YOLO('best.pt')

results = model('tcp://127.0.0.1:8888', stream=True)

for result in results:
  boxes = result.boxes
  probs = result.probs
  for box in boxes:
    detection_count = box.shape[0]
    for i in range(detection_count):
      cls = int(result.boxes.cls[i].item())
      name = result.names[cls]
      confidence = float(result.boxes.conf[i].item())
      bounding_box = result.boxes.xyxy[i].cpu().numpy()
