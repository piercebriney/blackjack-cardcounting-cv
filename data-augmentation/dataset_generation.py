from PIL import Image, ImageEnhance, ImageOps, ImageDraw, ImageFilter
import random
import numpy as np
from multiprocessing import Pool
import os
import string

class_id_mapping = {
    'a': 0,
    '2': 1,
    '3': 2,
    '4': 3,
    '5': 4,
    '6': 5,
    '7': 6,
    '8': 7,
    '9': 8,
    '10': 9,
    'j': 10,
    'q': 11,
    'k': 12
}

def resize(background_image, card_image):
    background_width, background_height = background_image.size
    card_width, card_height = card_image.size
    aspect_ratio = min(background_width / card_width, background_height / card_height)
    new_size = (int(card_width * aspect_ratio), int(card_height * aspect_ratio))

    scale = random.uniform(0.04, 0.4)
    scaled_size = (int(new_size[0] * scale), int(new_size[1] * scale))

    return card_image.resize(scaled_size, Image.Resampling.LANCZOS)

def add_padding(background_image, card_image):
    background_width, background_height = background_image.size
    card_width, card_height = card_image.size

    width_padding = background_width//2 - card_width//2
    height_padding = background_height//2 - card_height//2
    return ImageOps.expand(
        card_image,
        border=(width_padding,height_padding,width_padding,height_padding),
        fill=(0,0,0,0)
    )

def rotate(image):
    rotation_angle = random.uniform(0, 360)
    return image.rotate(rotation_angle, expand=True)

def modify_brightness(image):
    brightness_factor = random.uniform(0.3, 1.8)
    enhancer = ImageEnhance.Brightness(image)
    return enhancer.enhance(brightness_factor)

def modify_contrast(image):
    contrast_factor = random.uniform(0.9, 1.1)
    enhancer = ImageEnhance.Contrast(image)
    return enhancer.enhance(contrast_factor)

def modify_saturation(image):
    saturation_factor = random.uniform(0.9, 1.1)
    enhancer = ImageEnhance.Color(image)
    return enhancer.enhance(saturation_factor)

def add_gaussian_blur(image):
    radius = min(np.random.exponential(1), 5)
    return image.filter(ImageFilter.GaussianBlur(radius=radius))

def add_gaussian_noise(image):
    np_image = np.array(image)
    row, col, ch = np_image.shape

    mean = 0
    var = min(np.random.exponential(2.5), 50)
    sigma = var ** 0.5

    gauss = np.random.normal(mean, sigma, (row, col, ch))

    noisy_image = np_image + gauss

    noisy_image = np.clip(noisy_image, 0, 255).astype(np.uint8)

    return Image.fromarray(noisy_image)

def adjust_saturation(image):
    saturation_factor = random.uniform(0.0, 2.0)
    enhancer = ImageEnhance.Color(image)
    return enhancer.enhance(saturation_factor)

def adjust_hue(image):
    hsv_image = image.convert('HSV')
    
    h, s, v = hsv_image.split()

    hue_change = random.uniform(-10, 10)

    h = h.point(lambda i: (i + hue_change) % 255)

    hsv_adjusted = Image.merge('HSV', (h, s, v))

    return hsv_adjusted.convert('RGB')

def add_random_tint(image):
    image = image.convert("RGB")
    r, g, b = image.split()

    r = r.point(lambda i: i * random.uniform(0.8, 1.2))
    g = g.point(lambda i: i * random.uniform(0.8, 1.2))
    b = b.point(lambda i: i * random.uniform(0.8, 1.2))

    return Image.merge('RGB', (r, g, b))

def add_salt_and_pepper_noise(image):
    max_amount = 0.0005
    scale = 0.00003
    amount = min(np.random.exponential(scale), max_amount)
    np_image = np.array(image)
    row, col, ch = np_image.shape

    num_salt = np.ceil(amount * row * col)
    num_pepper = np.ceil(amount * row * col)

    # Apply salt (white) noise
    coords = [np.random.randint(0, i - 1, int(num_salt)) for i in (row, col)]
    np_image[coords[0], coords[1], :] = 255

    # Apply pepper (black) noise
    coords = [np.random.randint(0, i - 1, int(num_pepper)) for i in (row, col)]
    np_image[coords[0], coords[1], :] = 0

    return Image.fromarray(np_image)

def draw_bounding_box(image, x_center, y_center, width, height):
    img_width, img_height = image.size
    left = int((x_center - width / 2) * img_width)
    top = int((y_center - height / 2) * img_height)
    right = int((x_center + width / 2) * img_width)
    bottom = int((y_center + height / 2) * img_height)

    draw = ImageDraw.Draw(image)
    draw.rectangle([(left, top), (right, bottom)], outline="red", width=2)

def draw_point(image, location):
    x, y = location
    radius = 5

    draw = ImageDraw.Draw(image)
    draw.ellipse((x - radius, y - radius, x + radius, y + radius), fill='green')

def find_coeffs(pa, pb):
    matrix = []
    for p1, p2 in zip(pa, pb):
        matrix.append([p1[0], p1[1], 1, 0, 0, 0, -p2[0]*p1[0], -p2[0]*p1[1]])
        matrix.append([0, 0, 0, p1[0], p1[1], 1, -p2[1]*p1[0], -p2[1]*p1[1]])

    A = np.matrix(matrix, dtype=float)
    B = np.array(pb).reshape(8)

    res = np.dot(np.linalg.inv(A.T * A) * A.T, B)
    return np.array(res).reshape(8)

def convert_to_yolo_format(coordinates, total_width, total_height):
    xs = [point[0] for point in coordinates]
    ys = [point[1] for point in coordinates]
    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)

    # Calculate the bounding box's width and height
    width = max_x - min_x
    height = max_y - min_y

    # Calculate the center of the bounding box
    x_center = min_x + width / 2
    y_center = min_y + height / 2

    # Normalize these values
    x_center /= total_width
    y_center /= total_height
    width /= total_width
    height /= total_height

    return x_center, y_center, width, height

def process_segment(args):
    image, x_start, x_end = args
    width, height = image.size
    left, top, right, bottom = width, height, 0, 0

    for x in range(x_start, x_end):
        for y in range(height):
            _, _, _, a = image.getpixel((x, y))
            if a != 0:
                left = min(left, x)
                top = min(top, y)
                right = max(right, x)
                bottom = max(bottom, y)

    return (left, top, right, bottom)

def find_corners_parallel(image, num_processes=8):
    width, height = image.size

    if image.mode != 'RGBA':
        image = image.convert('RGBA')

    # Splitting the image into segments
    segment_width = width // num_processes
    segments = [(image, i * segment_width, (i + 1) * segment_width) for i in range(num_processes)]

    with Pool(num_processes) as p:
        results = p.map(process_segment, segments)

    # Combine results from all segments
    left = min(r[0] for r in results)
    top = min(r[1] for r in results)
    right = max(r[2] for r in results)
    bottom = max(r[3] for r in results)

    return (left, top, right, bottom)

def transform_perspective(background_image, card_image, initial_width, initial_height):
    card_width, card_height = initial_width, initial_height
    background_width, background_height = background_image.size

    left_bound = background_width//2 - card_width//2
    right_bound = background_width//2 + card_width//2
    top_bound = background_height//2 - card_height//2
    bottom_bound = background_height//2 + card_height//2

    initial_top_left = (left_bound, top_bound)
    initial_top_right = (right_bound, top_bound)
    initial_bottom_right = (right_bound, bottom_bound)
    initial_bottom_left = (left_bound, bottom_bound)

    transformed_top_left_x = random.randint(left_bound, left_bound+card_width//4)
    transformed_top_left_y = random.randint(top_bound, top_bound+card_height//4)
    transformed_top_right_x = random.randint(right_bound-card_width//4, right_bound)
    transformed_top_right_y = random.randint(top_bound, top_bound+card_height//4)
    transformed_bottom_right_x = random.randint(right_bound-card_width//4, right_bound)
    transformed_bottom_right_y = random.randint(bottom_bound-card_height//4, bottom_bound)
    transformed_bottom_left_x = random.randint(left_bound, left_bound+card_width//4)
    transformed_bottom_left_y = random.randint(bottom_bound-card_height//4, bottom_bound)

    initial = [initial_top_left, initial_top_right, initial_bottom_right, initial_bottom_left]
    transformed = [(transformed_top_left_x, transformed_top_left_y),
         (transformed_top_right_x, transformed_top_right_y),
         (transformed_bottom_right_x, transformed_bottom_right_y),
         (transformed_bottom_left_x, transformed_bottom_left_y)]


    coeffs = find_coeffs(initial, transformed)

    card_image = card_image.transform((card_image.size), Image.PERSPECTIVE, coeffs, Image.Resampling.BICUBIC)

    return card_image

def get_annotation(card_image, card_type, off_x, off_y):
    width, height = card_image.size
    left, top, right, bottom = find_corners_parallel(card_image)
    x_center, y_center, bounding_box_width, bounding_box_height = convert_to_yolo_format([
        (left, top), (right, top), (right, bottom), (left, bottom)
    ], width, height)

    x_center += (off_x / width)
    y_center += (off_y / height)

    return [class_id_mapping[card_type], x_center, y_center, bounding_box_width, bounding_box_height]

def write_annotations(filename, annotations_array):
    with open(f"{filename}", "w") as file:
        for annotation in annotations_array:
            file.write(f"{annotation[0]} {annotation[1]} {annotation[2]} {annotation[3]} {annotation[4]}\n")

def overlay(top_image, background_image, off_x, off_y):
    top_left = (off_x, off_y)
    background_image.paste(top_image, top_left, top_image)

def card_occludes_other_cards(new_annotation, previous_annotations):
    if not new_annotation: return True

    _, new_x_center, new_y_center, new_bb_width, new_bb_height = new_annotation
    for prev_annotation in previous_annotations:
        _, prev_x_center, prev_y_center, prev_bb_width, prev_bb_height = prev_annotation

        # Compute the coordinates of the intersection rectangle
        dx = min(new_x_center + new_bb_width / 2, prev_x_center + prev_bb_width / 2) - max(new_x_center - new_bb_width / 2, prev_x_center - prev_bb_width / 2)
        dy = min(new_y_center + new_bb_height / 2, prev_y_center + prev_bb_height / 2) - max(new_y_center - new_bb_height / 2, prev_y_center - prev_bb_height / 2)

        if (dx >= 0) and (dy >= 0):
            intersection_area = dx * dy
            prev_area = prev_bb_width * prev_bb_height

            # Check if more than 60% of the previous annotation is occluded
            if intersection_area / prev_area > 0.6:
                return True
    return False

def generate_image(card_image_paths, background_image_path, output_card_path, annotation_path):
    background_image = Image.open(background_image_path).convert("RGBA")
    background_width, background_height = background_image.size

    annotations_array = []
    for card_image_path in card_image_paths:
        card_image = Image.open(card_image_path).convert("RGBA")
        card_image = resize(background_image, card_image)
        card_image = rotate(card_image)

        initial_card_width, initial_card_height = card_image.size

        card_image = add_padding(background_image, card_image)
        card_image = transform_perspective(background_image, card_image, initial_card_width, initial_card_height)

        annotation = None
        attempts = 0
        solution_found = False
        while (not solution_found and attempts < 5):
            position_offset_x = random.randint(int(-0.3*background_width), int(0.3*background_width))
            position_offset_y = random.randint(int(-0.3*background_height), int(0.3*background_height))
            annotation = get_annotation(card_image, str.split(str.split(card_image_path, '/')[-1], '_')[0], position_offset_x, position_offset_y)
            solution_found = not card_occludes_other_cards(annotation, annotations_array)
            attempts += 1
        
        if (solution_found):
            annotations_array.append(annotation)
            overlay(card_image, background_image, position_offset_x, position_offset_y)
    
    write_annotations(annotation_path, annotations_array)

    background_image = add_random_tint(background_image) if random.randint(0,1) == 1 else background_image
    background_image = add_random_tint(background_image) if random.randint(0,1) == 1 else background_image
    background_image = modify_brightness(background_image) if random.randint(0,1) == 1 else background_image
    background_image = modify_contrast(background_image) if random.randint(0,1) == 1 else background_image
    background_image = modify_saturation(background_image) if random.randint(0,1) == 1 else background_image
    background_image = add_gaussian_noise(background_image) if random.randint(0,1) == 1 else background_image
    background_image = add_salt_and_pepper_noise(background_image) if random.randint(0,1) == 1 else background_image
    background_image = add_gaussian_blur(background_image) if random.randint(0,1) == 1 else background_image
    background_image = adjust_hue(background_image) if random.randint(0,1) == 1 else background_image
    background_image = adjust_saturation(background_image) if random.randint(0,1) == 1 else background_image
    background_image = background_image.convert("RGB")

    background_image.save(output_card_path, 'JPEG')

def random_string(length=12):
    letters = string.ascii_letters + string.digits
    return ''.join(random.choice(letters) for _ in range(length))

def generate_dataset(num_images_per_card):
    cards_directory = 'combined_decks'
    backgrounds_directory = 'backgrounds'
    output_image_directory = '../datasets/cards_2/images/val'
    annotations_directory = '../datasets/cards_2/labels/val'

    if not os.path.exists(cards_directory):
        os.makedirs(cards_directory)
    if not os.path.exists(backgrounds_directory):
        os.makedirs(backgrounds_directory)
    if not os.path.exists(output_image_directory):
        os.makedirs(output_image_directory)
    else:
        for filename in os.listdir(output_image_directory):
            file_path = os.path.join(output_image_directory, filename)
            try:
                if os.path.isfile(file_path):
                    os.remove(file_path)
            except Exception as e:
                print(f'Failed to delete {file_path}. Reason: {e}')
    if not os.path.exists(annotations_directory):
        os.makedirs(annotations_directory)
    else:
        for filename in os.listdir(annotations_directory):
            file_path = os.path.join(annotations_directory, filename)
            try:
                if os.path.isfile(file_path):
                    os.remove(file_path)
            except Exception as e:
                print(f'Failed to delete {file_path}. Reason: {e}')

    backgrounds = [f for f in os.listdir(backgrounds_directory) if os.path.isfile(os.path.join(backgrounds_directory, f))]
    def get_background():
        return random.choice(backgrounds)
    
    cards = [f for f in os.listdir(cards_directory) if os.path.isfile(os.path.join(cards_directory, f))]
    def get_card():
        return random.choice(cards)

    generated = 0
    for filename in os.listdir(cards_directory):
        if os.path.isfile(os.path.join(cards_directory, filename)):
            for _ in range(num_images_per_card):
                input_card_paths = [f'{cards_directory}/{filename}']
                num_cards = random.randint(1,3)
                for _ in range(num_cards - 1):
                    input_card_paths.append(f'{cards_directory}/{get_card()}')                    
                background_path = f'{backgrounds_directory}/{get_background()}'
                output_string = random_string()
                output_card_path = f'{output_image_directory}/{output_string}.jpg'
                annotation_path = f'{annotations_directory}/{output_string}.txt'
                generate_image(input_card_paths, background_path, output_card_path, annotation_path)
                generated += 1
                print(f'\rCompletion: {100 * (generated / (num_images_per_card*len(cards))):.2f}%', end='')

# generates 20 images per card (104 cards)
generate_dataset(20)