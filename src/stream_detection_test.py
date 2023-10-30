#call this first
#libcamera-vid -n -t 0 --width 1280 --height 960 --framerate 1 --inline --listen -o tcp://127.0.0.1:8888

from ultralytics import YOLO
import numpy

model = YOLO('best.pt')

results = model('tcp://127.0.0.1:8888', stream=True)

firstFrame = 1
objectsDetectedLastFrame = []
objectsDetectedThisFrame = []
counter = 0

allCards = ["4S", "3C", "6H", "4H", "3D"]
detectedCards = []
undetectedCards = []
for entry in allCards:
  undetectedCards.append(entry)

for result in results:
  print("     ")
  print("     ")
  print("-----")

  objectsDetectedThisFrame.clear()

  boxes = result.boxes
  probs = result.probs
  for box in boxes:
    detection_count = box.shape[0]
    for i in range(detection_count):
      cls = int(box.cls[i].item())
      name = result.names[cls]
      objectsDetectedThisFrame.append(name)
      confidence = float(result.boxes.conf[i].item())
      bounding_box = result.boxes.xyxy[i].cpu().numpy()

  
  if(firstFrame == 0):
    a_set = set(objectsDetectedLastFrame)
    b_set = set(objectsDetectedThisFrame)
    consistentlyDetectedCards = list(a_set.intersection(b_set))
    #a card in consistentlyDetected_set was detected over two consecutive frames
    for card in consistentlyDetectedCards:
      if card not in detectedCards:
        detectedCards.append(card)
      if card in undetectedCards:
        undetectedCards.remove(card)
       

  firstFrame = 0
  objectsDetectedLastFrame.clear()
  for entry in objectsDetectedThisFrame: 
    objectsDetectedLastFrame.append(entry)


  print("Played Cards:")
  print(detectedCards)
  print("Cards in Deck:")
  print(undetectedCards)
 
