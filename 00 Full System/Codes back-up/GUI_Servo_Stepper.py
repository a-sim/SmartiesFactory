from pymata_aio.pymata3 import PyMata3
import pyfirmata
import time
import random
from PyQt5 import QtWidgets, uic

# Configurations
board = PyMata3(2)
Servo_dispenser = 6  # servo attached to this pin
Servo_Sorter=7
Servo_Corking=8
Servo_Ejection=9
Stepper_table=5
Stepper_dir=4
Push_Bottom=True
board.send_reset()
board.sleep(.5)

#Initialization
board.servo_config(Servo_dispenser)
board.servo_config(Servo_Sorter)
board.servo_config(Servo_Corking)
board.servo_config(Servo_Ejection)

board.analog_write(Servo_dispenser, 0)
board.analog_write(Servo_Sorter, 0)
board.analog_write(Servo_Corking, 0)
board.analog_write(Servo_Ejection, 0)

LastStepperSpeed=0.001
time.sleep(5)

#Functions


def Move_Servo1_open():
    for ii in range(0,181,1):
        board.analog_write(Servo_dispenser, ii)
        time.sleep(0.005)

def Move_Servo1_close():
    for ii in range(181, 1, -1):
        board.analog_write(Servo_dispenser, ii)
        time.sleep(0.005)


def Move_Servo2_open():
    board.analog_write(Servo_Sorter, 180)


def Move_Servo2_close():
    board.analog_write(Servo_Sorter, 0)


def Move_Stepper_CW():
    Stepper_Speed=dlg.doubleSpinBox.value()
    board.digital_write(Stepper_dir, 1)
    for ii in range(1,200,1):
        Stepper_Speed = dlg.doubleSpinBox.value()
        board.digital_write(Stepper_table,1)
        time.sleep(Stepper_Speed)
        board.digital_write(Stepper_table,0)
        time.sleep(Stepper_Speed)
        #if Push_bottom==False:
            #break
def GoStepper():
    board.digital_write(Stepper_dir, 1)
    for ii in range (1,202,1):
        Stepper_Speed = dlg.doubleSpinBox.value()

        board.digital_write(Stepper_table, 1)
        time.sleep(Stepper_Speed)
        board.digital_write(Stepper_table, 0)
        time.sleep(Stepper_Speed)

def Stop():
    pass

def Move_Stepper_CCW():
    Stepper_Speed=dlg.doubleSpinBox.value()
    board.digital_write(Stepper_dir, 0)
    for ii in range(1,200,1):
        board.digital_write(Stepper_table,1)
        time.sleep(Stepper_Speed)
        board.digital_write(Stepper_table,0)
        time.sleep(Stepper_Speed)
        #if Push_bottom==False:
            #break

def Dial_1():
    i = (dlg.dial.value())/-10000
    print(i)
    dlg.doubleSpinBox.setValue(i)


def phase1():
    pass


def phase2():
    pass

def phase3():
    pass
app=QtWidgets.QApplication([])
dlg=uic.loadUi("GUI.ui")


board.digital_write(4, Stepper_dir)

while Push_Bottom==False:
    Phase1()
    Phase2()
    Phase3()
    Phase4()
    Phase5()
    Phase6()


LastStepperSpeed=0.001
Speed=dlg.doubleSpinBox.value()
#dlg.pushButton_4.clicked.connect(Move_Servo)
dlg.radioButton_16.clicked.connect(Move_Servo1_open)
dlg.radioButton_19.clicked.connect(Move_Servo1_close)
dlg.radioButton_17.clicked.connect(Move_Servo2_open)
dlg.radioButton_18.clicked.connect(Move_Servo2_close)
dlg.radioButton.clicked.connect(Move_Stepper_CW)
dlg.radioButton_2.clicked.connect(Move_Stepper_CCW)
dlg.dial.valueChanged.connect(Dial_1)
dlg.pushButton_6.clicked.connect(GoStepper)
dlg.doubleSpinBox.setValue(LastStepperSpeed)


dlg.show()
app.exec()