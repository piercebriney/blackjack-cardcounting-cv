from ultralytics import YOLO
import cv2

model = YOLO('runs/detect/train/weights/best.pt')

image = cv2.imread('test_card.PNG')

results = model.predict(image)

for detection in results:
    x1, y1, x2, y2, confidence, class_id = detection
    cv2.rectangle(image, (x1, y1), (x2, y2), (0, 255, 0), 2)
    cv2.putText(image, f"{class_id}", (x1, y1-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

cv2.imwrite("output_test.PNG", image)