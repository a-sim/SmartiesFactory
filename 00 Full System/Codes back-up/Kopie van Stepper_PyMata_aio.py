

"""
This file demonstrates using PyMata to control a stepper motor. It requires the use of the FirmataPlus
Arduino sketch included with this release.
It is based upon the following tutorial: https://learn.adafruit.com/adafruit-arduino-lesson-16-stepper-motors/overview
"""


from pymata_aio.pymata3 import PyMata3
import pyfirmata
import time

# create a PyMata instance
# ping callback function


# create a PyMata instance
# create a PyMata instance
#firmata = PyMata3(2)
firmata = pyfirmata.Arduino('COM6')

# send the arduino a firmata reset
#firmata.send_reset()

# configure the stepper to use pins 9.10,11,12 and specify 512 steps per revolution
#firmata.stepper_config(1024, [9, 10, 11, 12])

# allow time for config to complete
#firmata.sleep(.5)


# move motor #0 500 steps forward at a speed of 20
#firmata.stepper_step(20, 2500)

# firmata.sleep(4)
firmata.digital[4].write(0)


for ii in range(1,10000,1):
    print(ii)
    firmata.digital[5].write(1)
    time.sleep(0.00005)
    firmata.digital[5].write(0)
    time.sleep(0.00005)
# move motor #0 500 steps reverse at a speed of 20
# firmata.stepper_step(20, -500)

# close firmata
