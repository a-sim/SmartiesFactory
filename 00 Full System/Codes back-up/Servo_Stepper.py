from pymata_aio.pymata3 import PyMata3
import pyfirmata
import time
import random


board = PyMata3(2)
SERVO_MOTOR = 6  # servo attached to this pin
board.send_reset()

board.sleep(.5)
board.servo_config(SERVO_MOTOR)

board.analog_write(SERVO_MOTOR, 0)
Stepper_Speed=0.01
time.sleep(5)
board.digital_write(4, 0)

while True:

    for ii in range(1,30,1):
        print(ii)
        board.digital_write(5,1)
        time.sleep(Stepper_Speed)
        board.digital_write(5,0)
        time.sleep(Stepper_Speed)


    print('Servo=0')
    board.analog_write(SERVO_MOTOR, 0)
    time.sleep(2)

    for ii in range(1,30,1):
        print(ii)
        board.digital_write(5,1)
        time.sleep(Stepper_Speed)
        board.digital_write(5,0)
        time.sleep(Stepper_Speed)

    print('Servo=180')
    board.analog_write(SERVO_MOTOR, 180)
    time.sleep(2)

board.shutdown()