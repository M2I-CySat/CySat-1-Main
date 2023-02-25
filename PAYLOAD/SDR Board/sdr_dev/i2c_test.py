#!/usr/bin/python

import ctypes
#import pylibi2c
from smbus2 import SMBus, i2c_msg, SMBusWrapper
import time
import serial

def main():
    with open("/home/analog/test_i2c.txt", "a") as f:
        f.write(str(time.time()) + "It worked!")
    # I2C commands
    i2c_record_start = 0x01
    i2c_record_stop = 0x02
    i2c_shutdown = 0x03

    # Setup I2C device
    # Open i2c device /dev/i2c-0, addr 0x50.
    # i2c = pylibi2c.I2CDevice('/dev/i2c-0', 0x50)
    # # Set delay (ms)
    # i2c.delay = 1
    # # Set page_bytes
    # i2c.page_bytes = 16
    data = '0xDEADBEEF'
    # Set buffer size for write
    #buf = bytes(bytearray(256))

    # while(1):
    with serial.Serial('/dev/ttyPS0', 115200, timeout=1) as ser:
    #with SMBusWrapper(0) as bus:
        # # Read 64 bytes from address 80
        # msg = i2c_msg.read(80, 64)
        # bus.i2c_rdwr(msg)

        # Write some bytes to address 80
        while True:
            #msg = i2c_msg.write(0, bytes(data.encode("ascii")))
            #bus.i2c_rdwr(msg)
    	    print(ser.read(4))
	    time.sleep(1)
	    ser.write(b'hello from i2c_test\r\n')

	ser.close()
        # From i2c 0x0(internal address) read 1 byte data, using ioctl_read.
        # data = i2c.ioctl_read(0x0, 1)
        # Write data to i2c, buf must be read-only type
        # size = i2c.write(0x0, bytes(data.encode("ascii")))
        # print('data {}').format(data)
        # if data != 0:
        #     print('\n\ngood data: {}').format(data)
        #     time.sleep(2)


if __name__ == "__main__":
    main()
