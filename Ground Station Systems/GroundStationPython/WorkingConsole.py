#!/usr/bin/python3
import serial 
import time
import threading
from threading import Thread

#PORT SETTINGS
PORT = 'COM4'
BAUDRATE = 115200

def main():

    #init uart coms
    uart = uart_init()

    thread = threading.Thread(target=uart_read, args=(uart,))
    thread.daemon = True
    thread.start()

    words = input("Please write a message: ")
    while(uart != None and words != "quit"):
        uart_write(uart, words)
        words = input("Please write a message: ")

def uart_init():
    try:
        uart = serial.Serial(PORT, BAUDRATE, timeout=1)
    except:
        print("No SDR Port Connected")
        return
    if(uart.is_open):
        print("SDR UART Port Open")
    else:
        print("SDR UART Error! (Port probably not configured properly)")
    return uart


def uart_write(uart, msg):
    try:
        cmd = msg.encode()
        num_bytes = uart.write(cmd)
        print("TX(" + str(num_bytes) + "): " + msg)
    except Exception as e:
        print(e)
        print("UART Write Error (Port probably not connected or configured properly)")


def uart_read(uart):
    while(True):
        try:
            read = uart.readline().decode()
            num_bytes = len(read)
            if(num_bytes > 0):
                print("RX(" + str(num_bytes) + "): " + read)
        except:
            print("UART Read Error (Port probably not connected or configured properly)")


if __name__ == '__main__':
    main()
