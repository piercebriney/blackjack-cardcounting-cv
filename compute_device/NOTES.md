# Technical Notes

## Overview

The program has the following jobs:

1. Process incoming video frames from the Pi with YOLOv8 to get card information.
2. Send blackjack actions to the Pi.
3. Reset the state when the Pi indicates a shuffle.

This is implemented in C++ for performance and easy integration with the simulator code. While it would've been way more convenient to just use Python and import the necessary packages, we found a YOLOv8 implementation that could outperform CUDA-powered PyTorch: https://github.com/triple-Mu/YOLOv8-TensorRT (several weeks went into testing other implementations using the ONNX runtime or OpenCV DNN; most of the time was spent debugging package version issues and linker errors, and they were ultimately rejected due to relatively poor performance). This repo was forked and stripped down to the bare essentials; in particular, the `csrc/detect/end2end` directory was adapated as the baseline for this codebase, and the TensorRT engine build scripts were copied into `engine`. Using TensorRT enables the use of larger YOLOv8 models at reasonable framerates; this isn't optimizing performance for the sake of optimizing performance (larger models translate to more accurate detections).

## File Layout

These are the essential files that are specific to this codebase (the other files are from the simulator/or the YOLOv8 implementation):
- `main.cpp`: This is the main loop that drives everything.
- `include/bridge.h`: This communicates with the Pi using a simple protocol over TCP.
- `include/fp.h`: This processes card detections.
- `include/game.h`: This wraps the simulator code in a nice interface.

## Processing Card Detections

To simplify card tracking, the program makes several assumptions:

1. Cards have very little overlap.
2. Cards will be continously visible for the duration of the blackjack round.
3. Cards will not move after being placed (unless they're being cleared after the round finishes).
4. The camera will not move.
5. The player will never split.
6. The dealer's cards will be at the opposite end of the player's cards.

To decisively perceive a card, we require:

1. The coordinates of the bounding box of the card do not change much between frames.
2. The card must be present for some number of frames.

To detect the first condition, we take a hint from YOLOv8 and use IOU (area of intersection over area of union between two rectangles in 2-D space). Since a higher IOU indicates a larger overlap, we consider the location of a card to be the same between two frames if the IOU of the respective bounding boxes is above 90%.

To detect the second condition, we maintain a list of overall objects detected. Each object has a counter that indicates the number of frames in which it was present. For each new frame, we:

1. Decrement the counter for each object in the overall list.
2. For each object detected in the new frame, attempt to associate it with an object in the overall list via IOU. If a match is found, bump the match's counter by 2. This effectively adds 1 for persistent objects. If there's no match, add the object to the overall list (with its counter set to 1).
3. Remove all objects in the overall list with a counter of 0.
4. Filter the overall list for objects with counters above the frame threshold. These are the decisive cards. To avoid repeatedly perceiving these cards, we add a flag to indicate that we've seen it already.

Using the previous assumptions, assigning the decisive cards to the dealer or player is very straightforward; we compute the center of the bounding box, then check if it lies in the top half of the frame or the bottom half; the top cards go to the dealer, and the bottom cards go to the player.


## Blackjack Logic
To compute player actions, we leverage simulator code. Cleanly importing the simulator code without blatantly copying files was deemed too difficult because the compute device has no way to support splits, so we went with the nuclear option of just copying every single file over, then manually removed the splitting logic from the simulator.

### Detecting Round Finish

To send the betting amount to the Pi, we need to know when the blackjack round is over. To detect this, we rely on the dealer to remove all cards from the table. If we've seen an empty table (i.e., no YOLOv8 detections) for some number of seconds, then we consider the round to be over. An alternative implementation would be to precisely track when a win occurs, but the simulator code didn't make this easily accessible, and it could also be wrong due to errors in card perception.

### Betting Amount Discrepancy

One unfortunate consequence of not tracking wins is that the compute device doesn't know how to adjust the player bankroll; to simplify things, we assume that the bankroll just never changes. We also make another assumption regarding bet size; it is usually near the minimum or maximum allowed value, so it isn't necessary to convey a wide range of values to the Pi; we just bet the minimum or maximum (round at the midpoint). While the fidelity and accuracy of the bet size is reduced, we decided that this was an acceptable trade-off.

### Checking Shuffle Intent
The task of checking if the Pi's user sees a shuffle presents a problem; reading from the TCP socket via `recv()` is a blocking system call. Since we're on a tight schedule to finish our computations before the next frame arrives, blocking is unacceptable. To solve this (and not deal with threading), we use the `MSG_DONTWAIT` flag when we call `recv()`; this will return `EAGAIN` or `EWOULDBLOCK` if the call would've blocked (this implies there's no shuffle intent). Since frames are processed several times a second, this check happens frequently enough for perceived latency from the button press to not be an issue.


