//
// Sorting machine v1.0
// A modulized system for sorting M&M or Skittles into six different bins based its color
//
// by ivc <ivc@x-pec.com> Nov 2012
//
#include "functions.h"

// settings
// pins for the servos, any digital will work
#define FEEDHWHEEL_PIN 12
#define CAROUSEL_PIN 13
// color light led
#define LEDLIGHT_PIN A1
// analog distance sensor (using ADC readout)
#define DISTANCESENSOR_PIN A0
// RGB LED pins
#define RED_PIN 6
#define GREEN_PIN 5
#define BLUE_PIN 3

// time values
// distance check: how long to wait between distance sensor checks
const int waitdistancesensor = 10;
// color sampling: time to wait between color sensor readings (3x readings performed)
const int waitcolorreadings = 30;

// distance sensor trigger values for feed wheel positioning
const int fardistance = 75;

void setup() {
  // initialize serial communication:
  Serial.begin(115200);  
  while (!Serial);
  Serial.println("Sorting Machine v1.0 - Welcome to calibration");
  Serial.println("");
  Serial.println("Feed the following sequence of items (Skittles or M&Ms) into the ");
  Serial.println("machine, position the feed wheel 45 degrees so the red is the first item");
  Serial.println("and type S to begin the calibration process.");
  Serial.println("");
  Serial.println("Items: red, green, blue, orange, yellow, purple OR a single color to better accuracy");
  Serial.println("");
  while ((char)Serial.read() != 'S');

  // setup counter registers for servos
  initServo();

  //temp
  //analogWrite(A3, 255);
}

void loop() {  
  // main loop of processes
  feedWheelCalibrate();
  feedWheelMonitor();
  statusIndicator();
}

