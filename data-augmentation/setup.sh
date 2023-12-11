#!/bin/bash

python3 -m venv ./venv
./venv/bin/pip install -r requirements.txt

source ".env"
settings_directory=$(yolo settings | grep -oP "Printing '\K[^']+" | sed 's/\x1B\[[0-9;]*[JKmsu]//g' | xargs dirname)
settings_file="$settings_directory/settings.yaml"
absolute_path=$(realpath "$DATASETS_ROOT")
awk -v newDir="$absolute_path" '
$1 == "datasets_dir:" {
    print "datasets_dir: " newDir;
    next;
}
{ print }
' "$settings_file" > temp_file && mv temp_file "$settings_file"