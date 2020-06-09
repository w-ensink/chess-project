import board
import busio
import digitalio
import os
import sys
import subprocess
import re

from adafruit_mcp230xx.mcp23017 import MCP23017


def reader():
    p = subprocess.Popen(['i2cdetect', '-y', '1'], stdout=subprocess.PIPE, )
    o = []
    for i in range(0, 9):
        line = str(p.stdout.readline())

        for match in re.finditer("[0-9][0-9]:.*[0-9][0-9]", line):
            o.append(match.group())
    sys.stdout.write("\r " + str(o))
    sys.stdout.flush()


state = "init"

while 1:
    if state == 'read':
        reader()

    if state == 'init':
        command = input(" -> ")

        if command == 'read':
            state = 'read'
            print("READ")
            reader()
        elif command == 'quit':
            print('Goodbye!')
            exit()