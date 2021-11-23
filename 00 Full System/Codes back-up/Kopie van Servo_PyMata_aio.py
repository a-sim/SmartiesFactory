#!/usr/bin/env python3


"""
This example illustrates manipulating a servo motor.
"""

from pymata_aio.pymata3 import PyMata3

# instantiate PyMata with a 2 second start up delay to allow an Uno to complete its reset
board = PyMata3(2)

SERVO_MOTOR = 5  # servo attached to this pin

# configure the servo
board.servo_config(SERVO_MOTOR)

for x in range(0, 3):
    # move the servo to 20 degrees
    board.analog_write(SERVO_MOTOR, 20)
    board.sleep(1)

    # move the servo to 100 degrees
    board.analog_write(SERVO_MOTOR, 100)
    board.sleep(1)

    # move the servo to 20 degrees
    board.analog_write(SERVO_MOTOR, 20)

# close the interface down cleanly
board.shutdown()