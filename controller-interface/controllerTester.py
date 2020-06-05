import time
import board
import busio
import digitalio

from adafruit_mcp230xx.mcp23017 import MCP23017

i2c = busio.I2C(board.SCL, board.SDA)

devs = [MCP23017(i2c, 0x20), MCP23017(i2c, 0x21), MCP23017(i2c, 0x22), MCP23017(i2c, 0x23)]
state = 'INIT'

dev = 0
pin_glob = 0


def run_tester(device, pin):
    global dev, devs, pin_glob
    devs[dev].get_pin(pin_glob).value = False
    devs[device].get_pin(pin).value = True

    print("Pin " + str(pin) + " of device " + str(device) + " is now high!")
    pin_glob = pin
    dev = device


while 1:
    command = input(" -> ")

    if command == 'start':
        run_tester(dev, pin_glob)
    elif command == 'test':
        dv = int(input(" Device: "))
        pn = int(input(" Pin: "))
        run_tester(dv, pn)
    elif command == 'next':
        pn = pin_glob
        if pn < 15:
            pn += 1
            run_tester(dev, pn)
        elif dev < 4 and pn < 15:
            pn = 0
            dv = dev + 1
            run_tester(dv, pn)
        else:
            print("Finished testing all pins. Do you want to return to dev0 pin0? (yes/No)")
            cmd = input(" --> ")
            if cmd == "yes":
                dv = 0
                pin_glob = 0
                run_tester(dv, pin_glob)
            else:
                print("Now exiting")
                exit()
    elif command == 'quit':
        print("Goodbye!")
        exit()

