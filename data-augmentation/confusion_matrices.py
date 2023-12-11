from ultralytics import YOLO
import torch
import numpy as np
import re
import gc
import os
from dotenv import load_dotenv

load_dotenv()

device = "0" if torch.cuda.is_available() else "cpu"
if device == "0":
    torch.cuda.set_device(0)

def replace(file_path, pattern, replacement):
    with open(file_path, 'r') as file:
        content = file.read()
    
    updated_content = re.sub(pattern, replacement, content)
    
    with open(file_path, 'w') as file:
        file.write(updated_content)

def write_matrix_to_file(matrix, directory, filename):
    os.makedirs(directory, exist_ok=True)

    with open(os.path.join(directory, filename), 'w') as file:
        for row in matrix:
            file.write(' '.join(map(str, row)) + '\n')

def process_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    nums_matrix = [[float(i) for i in line.split()] for line in lines]
    nums_matrix = [row[:-1] for row in nums_matrix[:-1]]

    for i in range(len(nums_matrix[0])):
        column_sum = 0
        for j in range(len(nums_matrix)):
            column_sum += nums_matrix[j][i]
        if column_sum > 0:
            for j in range(len(nums_matrix)):
                nums_matrix[j][i] = nums_matrix[j][i] / float(column_sum)
        
    transposed_matrix = np.transpose(np.array(nums_matrix))

    processed_lines = []
    for row in transposed_matrix:
        new_line = ''
        for number in row:
            new_line += str(number)
            new_line += ' '
        processed_lines.append(new_line)

    with open(file_path, 'w') as file:
        file.write('\n'.join(processed_lines))

dataset_path = os.getenv('DATASET_PATH')
min_angle = float(os.getenv('MIN_ANGLE'))
max_angle = float(os.getenv('MAX_ANGLE'))
angle_step = float(os.getenv('ANGLE_STEP'))
min_distance = float(os.getenv('MIN_DISTANCE'))
max_distance = float(os.getenv('MAX_DISTANCE'))
distance_step = float(os.getenv('DISTANCE_STEP'))
confusion_matrix_directory = 'confusion-matrices'

replace('yolo_config.yml', r'path: ./.*\n', f'path: {dataset_path}\n')

model = YOLO(os.getenv('MODEL_PATH'))
for angle in np.arange(min_angle, max_angle+angle_step, angle_step):
    for distance in np.arange(min_distance, max_distance+distance_step, distance_step):
        replace('yolo_config.yml', r'images/test/.*\n', f'images/test/{distance}_{angle}\n')
        metrics = model.val(data='yolo_config.yml', imgsz=1280, split='test', device=0, batch=16)
        matrix = metrics.confusion_matrix.matrix
        write_matrix_to_file(matrix, confusion_matrix_directory, f'{distance}_{angle}.txt')
        process_file(os.path.join(confusion_matrix_directory, f'{distance}_{angle}.txt'))
        gc.collect()