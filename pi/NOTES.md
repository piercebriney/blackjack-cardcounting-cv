# Technical Notes

## Camera

Originally, we were hosting the video using this:
```
libcamera-vid -n -t 0 --width 1280 --height 1280 --framerate 10 --inline --listen -o tcp://0.0.0.0:8888
```

This encodes the video stream using the H.264 compression standard. However, this has terrible latency and framerate; we measured up to 5 seconds of delay between frames observed by the camera and the frames rendered by the compute device, and we were only able to get roughly 2 FPS before the latency got even worse. Benchmarking the processing sections in the compute device code showed that OpenCV was _blocking_ on the frame read for several tens of milliseconds, indicating that frame packets were either not fully transmitted to the compute device or were still being processed by the kernel(s). Frame reads generally took less than 10 milliseconds if there were pending frames already buffered, so we assumed that large read times indicated the compute device was keeping up with the camera frames. Since this was out of our control, we desparately sought alternatives for better performance (a card counting device that plays 5 seconds behind reality is hot garbage).

After several miserable hours of forum-trawling and googling, we found https://github.com/raspberrypi/picamera2/blob/main/examples/mjpeg_server.py. The code in `camera.py` is pulled almost verbatim from here, with minor configuration changes to resolution and framerate. This encodes the video stream using MJPEG, which is light enough to stream footage in real-time. Thankfully, OpenCV's `VideoCapture` class accepted both formats without any changes on the compute device side (just change the URL from `tcp://PI_IP:8888` to `http://PI_IP:8000/stream.mjpg`).

Note that the Picamera2 example code is licensed under the BSD 2-Clause "Simplified" License, so `LICENSE` is included in this directory to respect that.

## GPIO Server

There's not much in `server.py`; it's a basic TCP server.

Since the protocol messages are just 2 bytes long, we use the `TCP_NODELAY` flag to guarantee that each call to `send()` will actually transmit a packet over the wire (otherwise, they could be buffered within the kernel). Here's a description of the protocol for messages from the compute device:
| Bytes (big-endian) | Action      |
|--------------------|-------------|
| 0x0000             | Hit         |
| 0x0100             | Stay        |
| 0x0300             | Double-down |
| 0x0400             | Surrender   |
| 0x6900             | Bet low     |
| 0x6901             | Bet high    |

The usage of 2 bytes is a vestige of when we were sending integer multiples of the bet multiplier rather than just high or low; the second byte was used by the betting messages to store the multiplier (the maximum multiplier was 100, which fits in a byte).

We also experimented with different ways of transmitting information via the vibrator during the multiplier phase. For example, we tried encoding the multiplier in binary, with long vibrations indicating 1s and short vibrations indicating 0s (one byte is 8 bits). However, this was deemed too complex and error-prone for the user to reliably decode.

For signifying a shuffle, we just send a single null byte to the compute device. The value doesn't even need to be checked; the corresponding `recv()` will return with a length of 1, which is enough to convey the intent to shuffle.

