
from pymata_aio.pymata3 import PyMata3
import time
import random

# instantiate PyMata with a 2 second start up delay to allow an Uno to complete its reset
board = PyMata3(2)

SERVO_MOTOR = 6  # servo attached to this pin

board.send_reset()

# configure the stepper to use pins 9.10,11,12 and specify 512 steps per revolution
#board.stepper_config(512, [12, 11, 10, 9])

# allow time for config to complete
board.sleep(.5)
# configure the servo




board.servo_config(SERVO_MOTOR)

'''

for d in range(1,10):
    for x in range(0, 100):
     time.sleep(0.015)
    # move the servo to 20 degrees
     board.analog_write(SERVO_MOTOR, x)
     time.sleep(0.015)
     board.stepper_step(20, 50)
     time.sleep(0.015)

    # move the ser0vo to 100 degrees

    # move the servo to 20 degrees

'''



# send the arduino a firmata reset
board.analog_write(SERVO_MOTOR, 0)

time.sleep(5)

for d in range(1,20):
 # move motor #0 500 steps forward at a speed of 20
 g = random.randint(0, 1000)
 print ('stepper=')
 print(g)
 board.stepper_step(20, g)
 time.sleep(3)
# move the servo to 20 degrees

 g=random.randint(0, 180)
 print('Servo=')
 print(g)
 board.analog_write(SERVO_MOTOR, g)
 time.sleep(0.015)
# firmata.sleep(4)

# move motor #0 500 steps reverse at a speed of 20
# firmata.stepper_step(20, -500)


# close the interface down cleanly
board.shutdown()