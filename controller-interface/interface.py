# ChessController Interface script
# © 2020 Daniël Kamp @ HKU University of the Arts Utrecht, the Netherlands
#
# With inspiration from https://learn.adafruit.com/basic-resistor-sensor-reading-on-raspberry-pi/basic-photocell-reading

import time
import board
import busio
import digitalio
import argparse
import random

from pythonosc import udp_client
from adafruit_mcp230xx.mcp23017 import MCP23017

i2c = busio.I2C(board.SCL, board.SDA)

# Connections
devs = [MCP23017(i2c, 0x20), MCP23017(i2c, 0x21), MCP23017(i2c, 0x22), MCP23017(i2c, 0x23)]
pins = []

# Setup for OSC
parser = argparse.ArgumentParser()
parser.add_argument("--ip", default="127.0.0.1",
                    help="The ip of the OSC server")
parser.add_argument("--port", type=int, default=5005,
                    help="The port the OSC server is listening on")
args = parser.parse_args()

client = udp_client.SimpleUDPClient(args.ip, args.port)

# Environment variables
thresh = 5
scanned = False

# Runtime variables
readout = []
old_readout = []

for dev in devs:
    for i in range(15):
        pins.append(dev.get_pin(i))

for pin in pins:
    pin.pull = digitalio.Pull.UP


# Handle an open square
def open_square(p):
    pindex = pins.index(p)
    readout[pindex] = 0

    # If the square was previously not open...
    if old_readout[pindex] is not 0:
        # ...we send out an OSC message
        client.send_message("/pull", pindex)


# Handle a closed square
def close_square(p):
    pindex = pins.index(p)
    readout[pindex] = 1

    # If the square was previously not closed...
    if old_readout[pindex] is not 1:
        # ...we send out an OSC message
        client.send_message("/put", pindex)


while 1:

    # Backup and reset the readout array for the next iteration of the process
    old_readout = readout
    readout = []

    # Loop over all defined pins
    for pin in pins:
        pin.reading = 0

        # Set all pins as outputs...
        pin.direction = digitalio.Direction.OUTPUT
        # ...and set their value low
        pin.value = False

    # Wait 100ms before entering the second loop
    time.sleep(0.1)

    for pin in pins:
        # Set all pins as inputs
        pin.direction = digitalio.Direction.INPUT

        # Increase the reading until 1uF is reached on the cap
        # (Resistance on the LDR will cause this to take longer or shorter, dependent on the availability of light)
        while pin.value is False:
            pin.reading += 1

        if pin.reading < thresh:
            # Trigger the open_square function when the sensor gets enough light
            print("Open square at index: " + str(pins.index(pin)))
            open_square(pin)
        else:
            # Else, trigger the close_square function
            close_square(pin)
