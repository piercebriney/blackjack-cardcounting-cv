# Pi
This directory contains Python scripts for operating the camera and GPIO pins on the Raspberry Pi 4 (`camera.py` and `server.py`, respectively).

## Installation

### Prerequisites
- Raspberry Pi OS 64-bit
- Python >= 3.10
- Picamera2
- Buzzer on GPIO pin 21
- Button on GPIO pin 22

The scripts require Python packages; to install them, run:
```
pip3 install -r requirements.txt
```
In case Picamera2 isn't already on the system, run:
```
sudo apt-get update -y
sudo apt-get install python-picamera python3-picamera -y
```

## Usage

To start the camera server, run:
```
python3 camera.py
```
To start the GPIO server, run:
```
python3 server.py
```
Note that both of these are blocking commands (they will run indefinitely), so two SSH sessions are advised (one for each command).

Once the servers are up, the camera feed will be available at `http://PI_IP:8000/stream.mjpg` where `PI_IP` is the IPv4 address of the Pi, and the GPIO server will accept TCP connections on port 8889. Consult the documentation in the `compute_device` directory for further instructions.

