# Simulator

This tool simulates rounds of blackjack and calculates the expected profit of counting cards with errors in card perception.

## Installation

### Prerequisites
- Compiler that supports C++17
- CMake >= 3.19
- OpenMP >= 3.0

On a modern Debian-based system, you can run the following commands to install the prerequisites:
```
sudo apt-get update -y
sudo apt-get install build-essential cmake libomp-dev -y
```

A `Dockerfile` is also provided; to run the simulator in Docker, run:
```
docker build -t simulator .
docker run -it simulator
# this is inside the container
./build/simulator --help
```

### Building
Run the following commands while in the `simulator` directory:
```
mkdir build && cd build
cmake ..
make
```
This will produce an executable named `simulator` in the `build` directory.

## Usage

View all available options:
```
$ ./simulator --help
Usage: simulator arg_0  [options...]
            arg_0 : The path(s) to the confusion matrix to test, space-separated [required]

Options:
         --rounds : The number of rounds of blackjack to play per trial [default: 800]
         --trials : The number of trials to run [default: 10000]
    --perfectness : The interpolation value in the interval [0.0, 1.0] to the identity matrix [default: 0]
          --decks : The number of decks [default: 6]
           --seed : The seed to drive the RNG; a value of 0 will use a randomized seed [default: 1]
            --min : The minimum betting amount in dollars [default: 25]
            --max : The maximum betting amount in dollars [default: 2500]
       --bankroll : The starting bankroll amount in dollars [default: 10000]
           --save : Save the raw profits to a file named <matrix path>.output [implicit: "true", default: false]
        -?,--help : print help [implicit: "true", default: false]
```

Run with the default parameters on a single confusion matrix:
```
$ ./simulator ../matrix/1.0_15.txt
Loading ../matrix/1.0_15.txt
Mean: -2754.80
Standard deviation: 671.73
Probability of profit: 0.00
```

Run with the default parameters on several confusion matrices:
```
$ ./simulator ../matrix/1.0_15.txt ../matrix/1.0_90.txt
Loading ../matrix/1.0_15.txt
Mean: -2754.80
Standard deviation: 671.73
Probability of profit: 0.00

Loading ../matrix/1.0_90.txt
Mean: 974.14
Standard deviation: 3284.49
Probability of profit: 0.58
```

Run with a perfect confusion matrix (i.e., the identity matrix) and a randomized seed:
```
$ ./simulator ../matrix/1.0_15.txt --perfectness 1.0 --seed 0
Loading ../matrix/1.0_15.txt
Mean: 937.41
Standard deviation: 3344.33
Probability of profit: 0.58

$ ./simulator ../matrix/1.0_15.txt --perfectness 1.0 --seed 0
Loading ../matrix/1.0_15.txt
Mean: 985.81
Standard deviation: 3321.77
Probability of profit: 0.58
```
