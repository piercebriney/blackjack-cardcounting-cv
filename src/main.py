from pi_io import input, output
import time

def callback():
	output.output_signal_sync(23, 1)

input.listen_to_input(2, 1, callback)

time.sleep(1000000)