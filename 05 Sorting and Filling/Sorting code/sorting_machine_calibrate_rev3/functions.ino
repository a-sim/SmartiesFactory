//
// Sorting machine - functions
//
#include "functions.h"

int readDistanceSensor()
{
  // read the sensor
  int sensorreading = analogRead(DISTANCESENSOR_PIN);
  return sensorreading;
}

int getTravelTime(ItemColor prevcolor, ItemColor nextcolor) {
  // carousel drop area is a circle starting at one end with: red, green, blue, orange, yellow, purple
  int time = 0;
  int traveldiff = 0;

  // how far does the carousel need to move
  traveldiff = abs(prevcolor-nextcolor);
  
  // servo specs says 1.56s for a full 360degree rotation, given 6 positions, makes 1.56s/6=260ms to move past one position
  time = traveldiff*260;
  
  return time;
}

void moveFeedWheel(FeedWheelState action)
{
  // feed wheel positions/speeds: stop, start, slow
  int wheelpos_value[3] = {1440, 1000, 1350};

  if (action == stopwheel) {
    setFeedWheelPos(wheelpos_value[0]);
  } else if (action == startwheel) {
    setFeedWheelPos(wheelpos_value[1]);
  } else if (action == slowwheel) {
    setFeedWheelPos(wheelpos_value[2]);
  }
}

void moveCarousel(ItemColor nextcolor) {
  // drop positions for items: unknown, red, green, blue, orange, yellow, purple
  int itempos_value[7] = {1975, 1975, 1775, 1575, 1350, 1150, 1000};

  // does this work?
  //setCarouselPos(itempos_value[nextcolor]);

  if (nextcolor == unknown) {
    setCarouselPos(itempos_value[0]);
  } else if (nextcolor == red) {
    setCarouselPos(itempos_value[1]);
  } else if (nextcolor == green) {
    setCarouselPos(itempos_value[2]);
  } else if (nextcolor == blue) {
    setCarouselPos(itempos_value[3]);
  } else if (nextcolor == orange) {
    setCarouselPos(itempos_value[4]);
  } else if (nextcolor == yellow) {
    setCarouselPos(itempos_value[5]);
  } else if (nextcolor == purple) {
    setCarouselPos(itempos_value[7]);
  }
}

void setFeedWheelPos(int val)
{
    // Set Pulse width to value
    setServoPos(0, val);
}

void setCarouselPos(int val)
{
    // Set Pulse width to value
    setServoPos(1, val);
}

void displayItemColor(ItemColor itemcolor)
{
  uint8_t rgbvalue[3] = {0, 0, 0};

  // set the analog adc values to illuminate the RGB LED: unknown, red, green, blue, orange, yellow, purple
  if (itemcolor == red) {
    rgbvalue[0] = 255;
    rgbvalue[1] = 0;
    rgbvalue[2] = 0;
  } else if (itemcolor == green) {
    rgbvalue[0] = 0;
    rgbvalue[1] = 255;
    rgbvalue[2] = 0;
  } else if (itemcolor == blue) {
    rgbvalue[0] = 0;
    rgbvalue[1] = 0;
    rgbvalue[2] = 255;
  } else if (itemcolor == orange) {
    rgbvalue[0] = 255;
    rgbvalue[1] = 75;
    rgbvalue[2] = 0;
  } else if (itemcolor == yellow) {
    rgbvalue[0] = 255;
    rgbvalue[1] = 200;
    rgbvalue[2] = 200;
  } else if (itemcolor == purple) {
    rgbvalue[0] = 150;
    rgbvalue[1] = 0;
    rgbvalue[2] = 255;
  } else if (itemcolor == white) {
    rgbvalue[0] = 255;
    rgbvalue[1] = 255;
    rgbvalue[2] = 255;
  }
   analogWrite(RED_PIN, rgbvalue[0]);
   analogWrite(GREEN_PIN, rgbvalue[1]);
   analogWrite(BLUE_PIN, rgbvalue[2]);
}

