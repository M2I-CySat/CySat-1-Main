import serial
import time

'''
port = serial.Serial("COM4", baudrate=115200)
while True:
    print(port.name)
    print("Connected")

    port.write('5test'.encode())
    print('5test'.encode())
    time.sleep(1)
'''
# Packet Testing
data = 'EPS Battery'
size = len(data)
start = '11111111'+data+'11001'
print(size)
print(start.encode())
