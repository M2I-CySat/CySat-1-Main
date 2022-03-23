#-*- coding: utf-8
#Radiometer Parsing Function

#This code shows an example of reading in and plotting data that is outputted from a GNURadio GRC file.  
#In this example a Total Power Radiometer is developed in GNURadio GRC and uses the File Sink function to store the data. 
#The plot then shows the total power output from the radiometer as a matched load is submerged in Liquid Nitrogen, 
#then Ice Water and then left to dry.
# - - -
#
### Read the data

# Import Needed functions

# Import needed libraries
from pylab import *
import pylab
import scipy
import numpy
import scipy.io as sio
import csv


# Use this to set the filename for the data file and CSV Calibration file.


tpr = 'tpr_2014.06.12.Lab0.dat'
calib = 'tpr_calib_2014.06.12.Lab0.csv'
x2_data = 'tpr_x2_2014.06.12.Lab0.csv'


# Uses SciPy to open the binary file from GNURadio


f = scipy.fromfile(open(tpr),dtype=scipy.float32)


# Because of the valve function in GNURadio, there are zeros that get added to the file.  We want to trim out those zeros.

# In[5]:

f = numpy.trim_zeros(f)


# Create an index array for plotting.  Also, since we know the interval the data is taken, we can convert this to an actual time.

# In[6]:

y = numpy.linspace(0,(len(f)*.5),numpy.size(f))


### Plot the data

# In[7]:

plot(y,f)
xlabel('Time (sec)')
ylabel('rQ Values')
title('rQ vs Time')
grid(True)

pylab.show()

# ## Calibration

# The rQ values are the raw values from the total power radiometer and are uncalibrated.  While the graph shows the change in the total power recorded and shows that the radiometer can detect changes in noise temperature, it has no other meaning than that.  What we want is to show what the total power is in relation to a noise temperature.  Since we have recorded the values of the rQ at fixed and known teperatures, we can create a calibration line and calibrate the radiometer.  For this experiment, we found that the following values matched our two known temperatures.
# 
# |rQ Value|X^2 Voltage|Temperature
# |--------|-----------------------
# |.0977   | 1.9617    |77 K
# |.1507   | 2.085     |273.15 K
# 
# We can now solve for y = mx + b since we have two equations and two unknowns.
# 
# To work with this, a calibration file is created.  This is a very simple CSV file that contains 3 values: The raw rQ value, the raw voltage from the square-law detector (discussed later) and the observed temperature.  The table above would then look like the following in the file.
# ```
# .0977,1.9617,77
# .1507,2.085,273.15
# ```
# - - -

# We need to read in the values from our CSV file that contains the values

# In[67]:

read_csv = open(calib, 'rb')
csvread = csv.reader(read_csv)
rQ_values = []
temp_values = []
voltage = []

for row in csvread:
    rQ,volt,temp = row
    rQ_values.append(float(rQ))
    voltage.append(float(volt))
    temp_values.append(float(temp))
read_csv.close()

a = numpy.array([[rQ_values[0],1.0],[rQ_values[1],1.0]],numpy.float32)
b = numpy.array([temp_values[0],temp_values[1]])

z = numpy.linalg.solve(a,b)
print z


# Now we apply these values to the array that holds our raw rQ values

g = f*z[0]+z[1]


# Now we can re-plot the graph but this time with the calibrated noise temperatures

plt.figure()
plot(y,g)
xlabel('Time')
ylabel('Noise Temperature (K)')
title('Temp vs Time')
grid(True)

pylab.show()

# This is looking better, but the time at the bottom doesn't have much meaning.  Since we know the sample rate of the Software Defined Radio, we can calculate the time interval between each sample.

# - - -
# # Square-law data
# 
# We now want to look at the data from the Square-Law detector to verify the operation of the SDR.  In the experiment that was conducted above, a power splitter was used to split the RF signal so that one went to the SDR and the other to a square-law detector (with a 3.1 dB loss though).  Therefore both data should be the same.  Let's read and then plot this data.

read_csv = open(x2_data, 'rb')
csvread = csv.reader(read_csv)
dummy = []
x2_voltage = []

for row in csvread:
    dummy,x2voltage = row    
    x2_voltage.append(float(x2voltage))
read_csv.close()


# Like the SDR data, we want to have a time reference at the bottom.

w = numpy.linspace(0,(len(x2_voltage)*.01),numpy.size(x2_voltage))

plt.figure()
plot(w,x2_voltage)
xlabel('Time (sec)')
ylabel('Voltage (V)')
title('X^2 Voltage vs Time (Noisy)')
grid(True)

pylab.show()

# The Square-law detector doesn't have a filter on it unlike the data we get from the SDR.  The GNURadio program takes the data and applies a Low Pass Filter to "clean up" the information.  We need to do the same with our Square-law data.

from scipy import signal
N=100
Fc=2000
Fs=1600
h=scipy.signal.firwin(numtaps=N, cutoff=40, nyq=Fs/2)
x2_filt=scipy.signal.lfilter(h,1.0,x2_voltage)


plt.figure()
plot(w,x2_filt)
xlabel('Time (sec)')
ylabel('Voltage (V)')
title('X^2 Voltage vs Time')
axis([0, 610, 1.94, 2.12])
grid(True)

pylab.show()

# Now we wish to calibrate this data as well.  We will use the same file and use the calibration points in that file.

a = numpy.array([[voltage[0],1.0],[voltage[1],1.0]],numpy.float32)
b = numpy.array([temp_values[0],temp_values[1]])

z = numpy.linalg.solve(a,b)
print z

x2_calib = x2_filt*z[0]+z[1]

plt.figure()
plot(w,x2_calib)
xlabel('Time (sec)')
ylabel('Voltage (V)')
title('X^2 Noise Temp vs Time')
axis([0, 610, 70, 300])
grid(True)

pylab.show()

# This looks to be the same as our SDR graph, but let's overlay them to make sure

plt.figure()
plot(w,x2_calib,'r',label='X^2')
plot(y,g,'b',label='SDR')
xlabel('Time (sec)')
ylabel('Voltage (V)')
title('Noise Temperature vs Time')
axis([0, 610, 70, 300])
grid(True)
legend(loc='lower right')


# We have some timeshift due to two reasons.  One, the timing isn't always perfect when starting the collection of the two data sets.  And two, we get a timeshift from filtering the square-law data

pylab.show()


