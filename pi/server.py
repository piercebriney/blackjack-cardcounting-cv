# from gpiozero import LED
from pi_io import input, output
from time import sleep
import socket
import sys
import subprocess

buzzer = 21
button = 22


def emit(on, off):
    output.output_signal_sync(buzzer, on)
    sleep(off)

def transmit_action(n):
    for _ in range(n):
        emit(0.3, 0.2)

def transmit_mul(n):
    if n == 0:
        emit(0.3, 0.2)
    else:
        for _ in range(3):
            emit(0.3, 0.2)


def handle_msg(msg):
    cmd = msg[0]
    val = msg[1]
    # enum action {hit, stay, split, doubledown, surrender, voidaction};
    if cmd == 0:
        print("hit")
        transmit_action(1)
    elif cmd == 1:
        print("stay")
        transmit_action(2)
    elif cmd == 2:
        assert False, "no splitting"
    elif cmd == 3:
        print("doubledown")
        transmit_action(3)
    elif cmd == 4:
        print("surrender")
        transmit_action(4)
    elif cmd == 5:
        assert False, "no void action"
    elif cmd == 0x69:
        print("betting", val)
        transmit_mul(val)
    else:
        assert False, "invalid command"

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(("0.0.0.0", 8889))
    s.listen()

    while True:
        print("waiting on accept")
        conn, _ = s.accept()
        input.listen_to_input(button, 1, lambda: conn.sendall(b"\x00"))
        while True:
            print("waiting on recv")
            msg = conn.recv(2)
            if len(msg) == 0:
                print("eof")
                break
            handle_msg(msg)
    
if __name__ == "__main__":
    main()
