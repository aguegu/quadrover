#!/usr/bin/env python
import minimalmodbus
import time

node = minimalmodbus.Instrument('/dev/ttyUSB0', 2) 
node.serial.parity = 'E'

#node.debug = True

print "eeprom: "

def getOffsets():
	x = node.read_register(0, 0, 3, True)
	y = node.read_register(1, 0, 3, True)
	z = node.read_register(2, 0, 3, True)
	return x, y, z

def setOffsets(x, y, z):
	node.write_register(0, x, 0, 6, True)
	time.sleep(0.1)
	node.write_register(1, y, 0, 6, True)
	time.sleep(0.1)
	node.write_register(2, z, 0, 6, True)
	time.sleep(0.1)


print "x: %+3d, y: %+3d, z: %+3d" % getOffsets() 
setOffsets(7, 9, 2)
