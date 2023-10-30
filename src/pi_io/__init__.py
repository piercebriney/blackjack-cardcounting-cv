import sys
import atexit

# Used for listening to simulated input
key_to_channel_map = {
	"a": 2,
	"b": 3,
	"c": 4,
	"d": 5,
	"e": 6,
	"f": 7,
	"g": 8,
	"h": 9,
	"i": 10,
	"j": 11,
	"k": 12,
	"l": 13,
	"m": 14,
	"n": 15,
	"o": 16,
	"p": 17,
	"q": 18,
	"r": 19,
	"s": 20,
	"t": 21,
	"u": 22,
	"v": 23,
	"w": 24,
	"x": 25,
	"y": 26,
	"z": 27,
}

if '-s' in sys.argv:
	GPIO_LIB = 'SimulRPi.GPIO'
	import SimulRPi.GPIO as GPIO
	GPIO.setkeymap(key_to_channel_map)
else:
	GPIO_LIB = 'RPi.GPIO'
	import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

def cleanup():
	GPIO.cleanup()

atexit.register(cleanup)