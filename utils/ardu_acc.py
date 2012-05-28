#!/usr/bin/env python

import serial
import pylab
import time
from collections import deque

class Acc(object):
	elements = 100
	time_threshold = 10
	last_shake = 0
	lastACC = [0,0,0]
	def __init__(self):
		self.acc_x = deque([0] * self.elements)
		self.acc_y = deque([0] * self.elements)
		self.acc_z = deque([0] * self.elements)

	def update(self,line):
		data = line.rstrip().split(';')
		if len(data) >= 3:
			dx = float(data[0])
			dy = float(data[1])
			dz = float(data[2])
			self.appendACC(dx,dy,dz)
			self.lastACC = [dx,dy,dz]

	def appendACC(self,x,y,z):
		self.acc_x.append(x)
		self.acc_y.append(y)
		self.acc_z.append(z)
		self.acc_x.popleft()
		self.acc_y.popleft()
		self.acc_z.popleft()

	def isShake(self):
		millis = int(round(time.time() * 1000))

		if (self.lastACC[2] > 3000.0) and ((millis-self.last_shake) > self.time_threshold):
			self.last_shake = millis
			return True
		return False



def main():	
	print "Setting enviroment up..."
	ser = serial.Serial('/dev/cu.usbserial-A600agms', 115200, timeout=1)
	a = Acc()
	fig = pylab.figure()

	ax = fig.add_subplot(311)
	ay = fig.add_subplot(312)
	az = fig.add_subplot(313)

	px, = ax.plot(a.acc_x)
	py, = ay.plot(a.acc_y)
	pz, = az.plot(a.acc_z)

	ax.set_ylim((-100,100))
	ay.set_ylim((-2800,2800))
	az.set_ylim((-2800,2800))

	px.set_ydata(a.acc_x)
	px.set_ydata(a.acc_y)
	px.set_ydata(a.acc_z)

	pylab.ion()
	pylab.show()

	num_shakes = 0

	print "Starting!"
	try:	
		if ser.isOpen():
			while True:
				a.update(ser.readline())
				px.set_ydata(a.acc_x)
				py.set_ydata(a.acc_y)
				pz.set_ydata(a.acc_z)
				pylab.draw()
				if a.isShake():
					print "SHAKEEE %d!" % (num_shakes)
					num_shakes += 1

		else:
			print "Cannot open SerialPort, quitting....."
	except:
		print "Bye!"
		ser.close()

if __name__ == "__main__": main()

