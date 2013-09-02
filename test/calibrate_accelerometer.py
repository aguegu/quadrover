#!/usr/bin/env python
import sys
import minimalmodbus
import time

node = minimalmodbus.Instrument('/dev/ttyUSB0', 2) 
node.serial.parity = 'E'

#node.debug = True

def getOffset(index):
	return node.read_register(index, 0, 3, True)

def getOffsets():
	return getOffset(0), getOffset(1), getOffset(2)

def setOffset(index, value):
	node.write_register(index, value, 0, 6, True)

def setOffsets(x, y, z):
	setOffset(0, x);
	setOffset(1, y);
	setOffset(2, z);

def getValue(index):
	return node.read_register(index, 0, 4, True)

def getValues():
	return getValue(0), getValue(1), getValue(2)

def getAverage(index, count):
	l = list()
	for i in range(0, count):
		l.append(getValue(index))
		sys.stdout.write("(%d/%d)\r" % (i + 1, count))
		sys.stdout.flush()
	
	print 
	return round(sum(l)/float(len(l)))

def getAverages(count):
	return getAverage(0, count), getAverage(1, count), getAverage(2, count)

print "Adxl345 Calibration:"
reply = raw_input("1. initialization: clear all offsets? [y/N] ")

if (reply.lower() == 'y'):
	setOffsets(0, 0, 0)

print "\toffsets: x: %+3d, y:%+3d, z: %+3d" % getOffsets()

raw_input("2. put Z+ in up direction: ")
z_positive = getAverage(2, 100)
print "\tZ+ : %+d" % z_positive

raw_input("3. put Z- in up direction: ")
z_negative = getAverage(2, 100)
print "\tZ- : %+d" % z_negative

raw_input("4. put X+ in up direction: ")
x_positive = getAverage(0, 100)
print "\tX+ : %+d" % x_positive

raw_input("5. put X- in up direction: ")
x_negative = getAverage(0, 100)
print "\tX- : %+d" % x_negative

raw_input("6. put Y+ in up direction: ")
y_positive = getAverage(1, 100)
print "\tY+ : %+d" % y_positive

raw_input("7. put Y- in up direction: ")
y_negative = getAverage(1, 100)
print "\tY- : %+d" % y_negative

x_offset = round(-(x_negative + x_positive) / 8.0)
y_offset = round(-(y_negative + y_positive) / 8.0)
z_offset = round(-(z_negative + z_positive) / 8.0)

print "X offseit: %+d" % x_offset
print "Y offseit: %+d" % y_offset
print "Z offseit: %+d" % z_offset

reply = raw_input("8. update offsets: [y/N] ")

if (reply.lower() == 'y'):
	setOffsets(x_offset, y_offset, z_offset)

print "\toffsets: x: %+3d, y:%+3d, z: %+3d" % getOffsets()


