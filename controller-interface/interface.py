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

from pythonosc.udp_client import SimpleUDPClient
from adafruit_mcp230xx.mcp23017 import MCP23017

i2c = busio.I2C(board.SCL, board.SDA)

# Connections
devs = [MCP23017(i2c, 0x22), MCP23017(i2c, 0x23)]
pins = []
lookup = [60, 52, 44, 36, 28, 20, 12, 4,
          61, 53, 45, 37, 29, 21, 13, 5,
          62, 54, 46, 38, 30, 22, 14, 6,
          63, 55, 47, 39, 31, 23, 15, 7]

# Setup for OSC
ip = "127.0.0.1"
port = 6001

client = SimpleUDPClient(ip, port)

# Environment variables
thresh = 15
scanned = False

# Runtime variables
readout = []
old_readout = []

for dev in devs:
    for i in range(16):
        pins.append(dev.get_pin(i))

for pin in pins:
    pin.pull = digitalio.Pull.UP


# Handle an open square
def open_square(p):
    pindex = pins.index(p)
    readout.append(0)

    # If the square was previously not open...
    if old_readout and old_readout[pindex] is not 0:
        print("Pulling from " + str(lookup[pindex]))
        # ...we send out an OSC message
        client.send_message("/pull", lookup[pindex])


# Handle a closed square
def close_square(p):
    pindex = pins.index(p)
    readout.append(1)

    # If the square was previously not closed...
    if old_readout and old_readout[pindex] is not 1:
        print("Putting to " + str(lookup[pindex]))
        # ...we send out an OSC message
        client.send_message("/put", lookup[pindex])


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
            open_square(pin)
        else:
            # Else, trigger the close_square function
            close_square(pin)
