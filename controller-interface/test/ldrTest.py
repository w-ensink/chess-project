import time
import board
import busio
import digitalio
import sys

from adafruit_mcp230xx.mcp23017 import MCP23017

i2c = busio.I2C(board.SCL, board.SDA)

devs = [MCP23017(i2c, 0x22), MCP23017(i2c, 0x23)]

# For testing:
# pins = [devs[0].get_pin(6), devs[0].get_pin(7)]

# For real:
matrix = {"a": [], "b": [], }
unknown = None
readout = []

pins = []

for dev in devs:
	for i in range(16):
		pins.append(dev.get_pin(i))

for pin in pins:
	pin.pull = digitalio.Pull.UP

while 1:
	readout = []
	for pin in pins:
		pin.reading = 0

		# setup
		pin.direction = digitalio.Direction.OUTPUT
		pin.value = False

	time.sleep(0.05)

	for pin in pins:

		pin.direction = digitalio.Direction.INPUT

		while pin.value is False:
			pin.reading += 1

		if pin.reading < 15:
			readout.append(0)
		else:
			readout.append(1)

	sys.stdout.write("\r " + str(readout))
	sys.stdout.flush()
