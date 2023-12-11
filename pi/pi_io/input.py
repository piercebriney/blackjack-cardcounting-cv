from . import GPIO_LIB
import importlib
import threading
import time

GPIO = importlib.import_module(GPIO_LIB)

def _gpio_listen(pin_number, min_duration_in_seconds, callback):
	GPIO.setup(pin_number, GPIO.IN, pull_up_down=GPIO.PUD_UP)

	while True:
		start_time = None
		ran_callback = False
		while GPIO.input(pin_number) == 0:
			if start_time is None:
				start_time = time.time()
			
			elapsed_time = time.time() - start_time
			if elapsed_time >= min_duration_in_seconds and not ran_callback:
				callback()
				ran_callback = True
			
			time.sleep(0.01)

		time.sleep(0.01)

def listen_to_input(pin_number, min_duration_in_seconds, callback):
	listener_thread = threading.Thread(target=_gpio_listen, args=(pin_number, min_duration_in_seconds, callback))
	listener_thread.start()