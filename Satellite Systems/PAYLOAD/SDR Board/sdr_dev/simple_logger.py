#!/usr/bin/python
import serial,os, time
#print'Executing GNURadio script...'
#os.system("simulated_noise_radiometer.py")
print'Opening Serial port and logging data'
 
s = serial.Serial("/dev/tty.usbserial-A40081C2", 19200)
filename = "square_law.csv"
fname, ext = os.path.splitext(filename)
timestamp = time.strftime('_%Y_%m_%d_%H_%M_%S')
new_filename = fname+timestamp+ext 
f = open(new_filename, "w")
 
while True:
        timestamp = time.strftime('%m/%d/%Y %H:%M:%S,')
        x = s.readline()
        x = timestamp + x
        print(x)      #Remove the comment to see output in monitor program
        f.write(x)
        f.flush()                      
        os.fsync(f)
 
f.close()
