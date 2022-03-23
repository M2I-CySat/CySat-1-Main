#
# Radiometer Logger script
#
# Written by Matthew Nelson
# Based on code written by Ted Burke
# Last updated 5/23/2012
#

import serial, glob

import wx
import numpy
import matplotlib
import sys
import time
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg
from matplotlib.figure import Figure
import matplotlib.pyplot as plt

#def scan():
   # scan for available ports. return a list of device names.
#   return glob.glob('/dev/ttyS*') + glob.glob('/dev/ttyUSB*')
#print "Found ports:"
#for name in scan(): print name

#serial = raw_input('Enter the name of the serial port (ex. /dev/ttyUSB0): ')

class DataLoggerWindow(wx.Frame):
	def __init__(self):
		wx.Frame.__init__(self, None, -1, "Radiometer", (100,100), (640,480))

		self.SetBackgroundColour('#ece9d8')

		# Flag variables
		self.isLogging = False

		# Create data buffers
		self.N = 100
		self.n = range(self.N)
		self.M = 3
		self.x = []
		for m in range(self.M):
			self.x.append(0 * numpy.ones(self.N, numpy.int))

		# Create plot area and axes
		self.fig = Figure(facecolor='#ece9d8')
		self.canvas = FigureCanvasWxAgg(self, -1, self.fig)
		self.canvas.SetPosition((0,0))
		self.canvas.SetSize((640,320))
		self.ax = self.fig.add_axes([0.08,0.1,0.86,0.8])
		self.ax.autoscale(True)
		self.ax.set_xlim(0, 99)
		self.ax.set_ylim(-50, 1100)
		for m in range(self.M):
			self.ax.plot(self.n,self.x[m])

		# Create text box for event logging
		self.log_text = wx.TextCtrl(
			self, -1, pos=(140,320), size=(465,100),
			style=wx.TE_MULTILINE)
		self.log_text.SetFont(
			wx.Font(12, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))

		# Create timer to read incoming data and scroll plot
		self.timer = wx.Timer(self)
		self.Bind(wx.EVT_TIMER, self.GetSample, self.timer)

		# Create start/stop button
		self.start_stop_button = wx.Button(
			self, label="Start", pos=(25,320), size=(100,100))
		self.start_stop_button.SetFont(
			wx.Font(14, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))
		self.start_stop_button.Bind(
			wx.EVT_BUTTON, self.onStartStopButton)

	def GetSample(self, event=None):
		# Get a line of text from the serial port
		sample_string = self.ser.readline()

		# Add the line to the log text box
		self.log_text.AppendText(sample_string)

		# If the line is the right length, parse it
		if len(sample_string) == 24:
			sample_string = sample_string[0:-1]
			sample_values = sample_string.split()

			for m in range(self.M):
				# get one value from sample
				value = int(sample_values[m])
				self.x[m][0:99] = self.x[m][1:]
				self.x[m][99] = value

			# Update plot
			self.ax.cla()
			self.ax.autoscale(True)
			self.ax.set_xlim(0, self.N - 1)
			self.ax.set_ylim(-50, 1100)
			for m in range(self.M):
				self.ax.plot(self.n, self.x[m])
			self.canvas.draw()

	def onStartStopButton(self, event):
		if not self.isLogging:
			self.isLogging = True
			try:
				self.ser = serial.Serial("/dev/ttyUSB0", 19200, 8, 'N', 1, timeout = 5)
			except:
				print "Error opening COM port.  Quitting."
				sys.exit(0)
			#self.ser = serial.Serial()
			#self.ser.baudrate = 19200
			#self.ser.timeout=0.25
			# We are going to assume a USBSerial adapter
			# which should be ttyUSBx, with x being the port number
			#self.ser.port = serial
			#for m in range(29, 0, -1):
				#self.ser.port = m
			#try:
					# Try this port number
					#self.ser.open()
					# We only get to here if port opened
					#self.log_text.AppendText(
					#	'Opened COM' + str(m+1) + '...\n')
					#break
				#except:
					# We end up here if this port number
					# failed to open
					#pass
			if self.ser.isOpen():
				# We successfully opened a port, so start
				# a timer to read incoming data
				self.timer.Start(100)
				self.start_stop_button.SetLabel("Stop")
		else:
			self.timer.Stop()
			self.ser.close()
			self.isLogging = False
			self.start_stop_button.SetLabel("Start")

if __name__ == '__main__':
	app = wx.PySimpleApp()
	window = DataLoggerWindow()

	window.Show()
	app.MainLoop()


