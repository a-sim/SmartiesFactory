//
// Sorting machine - functions
//

#ifndef SORTINGFUNC_H
  #define SORTINGFUNC_H

  #include "timer.h"
  #include "colorsensor.h"

  // data types
  typedef enum {none, startwheel, stopwheel, slowwheel} FeedWheelState;
  typedef enum {waiting, steady, blinking} StatusState;
  char* colortext[] = {"unknown", "red", "green", "blue", "orange", "yellow", "purple"};
  
  // global system variables
  ItemColor nextcolor = unknown;
  ItemColor prevcolor = unknown;
  int carouselmoving = 0;
  int outofposition = 0;
  int inposition = 0;
  StatusState statusindicator = waiting;
  ItemColor statuslight = unknown; // same as off

  // servo pins
  #if !defined(SERVO0_PIN)
   #define SERVO0_PIN FEEDHWHEEL_PIN
  #endif
  
  #if !defined(SERVO1_PIN)
   #define SERVO1_PIN CAROUSEL_PIN
  #endif
  
  int readDistanceSensor();
  int getTravelTime(ItemColor prevcolor, ItemColor);
  void moveFeedWheel(FeedWheelState);
  void moveCarousel(ItemColor);
  void setCarouselPos (int);
  void setFeedWheelPos (int);
  void displayItemColor(ItemColor);
#endif
