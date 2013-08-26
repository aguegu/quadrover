#!/usr/bin/env python
import minimalmodbus

node = minimalmodbus.Instrument('/dev/ttyUSB0', 2) 
node.serial.parity = 'E'

#node.debug = True

x = node.read_register(0, 0, 4, True)
y = node.read_register(1, 0, 4, True)
z = node.read_register(2, 0, 4, True)

print "x: %d, y: %d, z: %d" % (x, y, z)

'''
02050000ff008C09
020500000000CDf9

00050000ff008DEB
000500000000CC1B

'''
