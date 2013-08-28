#!/usr/bin/env python
import minimalmodbus

node = minimalmodbus.Instrument('/dev/ttyUSB0', 2) 
node.serial.parity = 'E'

#node.debug = True

def getAccelerometerValues():
	x = node.read_register(0, 0, 4, True)
	y = node.read_register(1, 0, 4, True)
	z = node.read_register(2, 0, 4, True)
	return x, y, z


print "Adxl345: "

count = 10
i = count;

x = 0
y = 0
z = 0

while (i > 0):
	xx, yy, zz = getAccelerometerValues()
	print "x: %+3d, y: %+3d, z: %+3d" % (xx, yy, zz)
	i -= 1;
	x += xx;
	y += yy;
	z += zz;

print "summary:"
print "x: %+3d, y: %+3d, z: %+3d" % (x / count, y / count, z / count)

