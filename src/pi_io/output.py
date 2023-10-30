from . import GPIO_LIB
import importlib
import threading
import time

GPIO = importlib.import_module(GPIO_LIB)

end_times = {}
def _gpio_output_check():
	while True:
		curr_time = time.time()
		for pin, end_time in list(end_times.items()):
			if curr_time >= end_time:
				GPIO.output(pin, GPIO.LOW)
				del end_times[pin]
			else:
				GPIO.output(pin, GPIO.HIGH)
		time.sleep(0.01)

output_thread = threading.Thread(target=_gpio_output_check)
output_thread.start()

def _add_output_to_queue(pin_number, duration_in_seconds):
	GPIO.setup(pin_number, GPIO.OUT)
	end_time = time.time() + duration_in_seconds
	end_times[pin_number] = max(end_times[pin_number], end_time) if pin_number in end_times else end_time

def output_signal_sync(pin_number, duration_in_seconds):
	_add_output_to_queue(pin_number, duration_in_seconds)
	time.sleep(duration_in_seconds)

def output_signal_async(pin_number, duration_in_seconds):
	_add_output_to_queue(pin_number, duration_in_seconds)