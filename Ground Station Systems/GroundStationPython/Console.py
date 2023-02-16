#!/usr/bin/python3
import serial
import time
import threading
import sys

port = "4"
baud = 115200
uart = serial.Serial('COM' + str(port), baud)
print("Press Enter to Begin")

def connect():
    """The function initiates the connection to the UART device (dev board) with the Port and Baud fed
    """
    print("Enter 1 for windows or 2 for Linux\n", input())
    os_type = input()

    try:
        if os_type == 2:
            try:
                uart = serial.Serial('/dev/tty' + str(port), baud)
            except:
                print
                "Cannot Open Port"
        elif os_type == 1:
            uart = serial.Serial('COM' + str(port), baud)
    except ValueError:
        print
        "Enter Baud and Port"
        return


def uart_write(uart, msg):
    cmd = msg + '\n'
    num_bytes = uart.write(cmd.encode()) - 1
    print("TX(" + str(num_bytes) + "): " + msg)


def uart_read(uart):
    read = uart.readline().decode('ascii').strip('\n')
    num_bytes = len(read)
    if num_bytes > 0:
        print("RX(" + str(num_bytes) + "): " + read)
    return read


if __name__ == "main":
    connect()
    uart_write(uart, "hello")
    print("Data:", uart_read(uart))