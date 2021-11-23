//
// Sorting machine - feedwheel process
//
#include "colorsensor.h"
#include "softservo.h"

void feedWheelCalibrate() {
  static int init = 0;
  static enum {feeditem, waitformovement, waitforfeed, performanalysis, showcalibration, donecalibration} state;
  static FeedWheelState wheelaction = none;
  static int waitpreemptmove = 0;
  static int colorsamples = 0;
  static int detectedrgbcolors[4] = {0, 0, 0, 0};
  static ItemColor founditemcolors[3] = {unknown, unknown, unknown};
  static ItemColor finalcolor = unknown;
  static Timer t = newTimer();
  static int calibratecolor = 0;
  static int calibrateround = 0;
  static int calibratedcolors[2][7][3][4]; // two rounds, seven colors, tree samples, four color components
  int k, i, j, lowestval = 1023, lowestval_idx;
  double hsv[3];
  double colorhue[3], colorsat[3], colorval[3];
  double colorsumhue[7], colorsumsat[7], colorsumval[7];

  // initialize
  if(!init) {
    //Serial.println("Initializing Feed Wheel Process");
    state = feeditem;
    // put the carousel exit tube in the middle facing the front
    moveCarousel(green);

    init = 1;
  }

  // switch the program into the proper state
  switch (state) {
  case feeditem:
    if (calibrateround < 2) {

      // set next color to calibrate
      if (calibratecolor < 6) {
        calibratecolor++; // starts off with element [1] red in colortext[] array
  
        // move feed wheel to release an item and load a new item
        wheelaction = startwheel;
        moveFeedWheel(wheelaction);
     
        // update status indicator
        statusindicator = steady;
        statuslight = (ItemColor)calibratecolor;
    
        //Serial.println("Feed Wheel: Moving the feed wheel, awaiting confirmation");
        state = waitformovement;
      } else { 
        wheelaction = stopwheel;
        moveFeedWheel(wheelaction);
        calibrateround++;
        calibratecolor = 0;
        statuslight = unknown;

        if (calibrateround < 2) {
          Serial.println("Next round, insert sequence red, green, blue, orange, yellow, purple");
          Serial.println("OR a single color to better accuracy, and type S:");
          Serial.println("");
          while ((char)Serial.read() != 'S');
        }
      }
    } else {
      calibrateround = 0;
      state = showcalibration;
    }
    break;
  case waitformovement:
    // wait for feedwheelmonitor to indicate the wheel is moving
    if (outofposition) {
       // clear status
       outofposition = 0;
 
       // sensor detected change from empty hole to wheel wall
      //Serial.println("Feed Wheel: Moving, waiting to be in position");
      state = waitforfeed;
    }
    break;
  case waitforfeed:
    // wait for feedwheelmonitor to indicate a new empty hole and stop the wheel
    if (inposition) {
      // clear status
      inposition = 0;

      // sensor detected change from wheel wall to hole, move slow to allow sensor to perform measurements
      //Serial.println("Feed Wheel: Stopping, wheel is in position to load and analyze new item");
      wheelaction = slowwheel;
      moveFeedWheel(wheelaction);
      statusindicator = steady;
      startTimer(1, t);

      state = performanalysis;
    }
    break;
  case performanalysis:
    // analyze the item, detect color three times while wheel is rotating slightly

    if (timeout(t)) {
      if (colorsamples < 3) {
        getColors(detectedrgbcolors);
        founditemcolors[colorsamples] = determineColor(detectedrgbcolors);
        calibratedcolors[calibrateround][calibratecolor][colorsamples][0] = detectedrgbcolors[0];
        calibratedcolors[calibrateround][calibratecolor][colorsamples][1] = detectedrgbcolors[1];
        calibratedcolors[calibrateround][calibratecolor][colorsamples][2] = detectedrgbcolors[2];
        calibratedcolors[calibrateround][calibratecolor][colorsamples][3] = detectedrgbcolors[3];
        
        // find the lowest value to use for base to get the difference
        for (i = 0; i < 3; i++) {
          if (detectedrgbcolors[i] < lowestval) {
            lowestval_idx = i;
            lowestval = detectedrgbcolors[i];
          }
        }
        Serial.print("Feed Wheel: Calibration of ");
        Serial.print(colortext[calibratecolor]);
        Serial.print(" sample #");
        Serial.print(colorsamples+1);
        Serial.print(": ");
        Serial.print(detectedrgbcolors[0]);
        Serial.print(", ");
        Serial.print(detectedrgbcolors[1]);
        Serial.print(", ");
        Serial.print(detectedrgbcolors[2]);
        Serial.print(" (");
        Serial.print(detectedrgbcolors[3]);
        Serial.print(") hsv: ");
        rgbToHSV((double)detectedrgbcolors[0], (double)detectedrgbcolors[1], (double)detectedrgbcolors[2], hsv);
        if (hsv[0] > 356) hsv[0] = 0;
        Serial.print(hsv[0]);
        Serial.print(" ");
        Serial.print(hsv[1]);
        Serial.print(" ");
        Serial.println(hsv[2]);
        /*
        Serial.print(") diff ");
        Serial.print(detectedrgbcolors[0]-detectedrgbcolors[lowestval_idx]);        
        Serial.print(", ");
        Serial.print(detectedrgbcolors[1]-detectedrgbcolors[lowestval_idx]);        
        Serial.print(", ");
        Serial.print(detectedrgbcolors[2]-detectedrgbcolors[lowestval_idx]);
        Serial.print("  ");
        if (detectedrgbcolors[0]-detectedrgbcolors[lowestval_idx] < detectedrgbcolors[1]-detectedrgbcolors[lowestval_idx]) {
          Serial.print((float)(detectedrgbcolors[0]-detectedrgbcolors[lowestval_idx])/(float)(detectedrgbcolors[1]-detectedrgbcolors[lowestval_idx]));
        } else {
          Serial.print((float)(detectedrgbcolors[1]-detectedrgbcolors[lowestval_idx])/(float)(detectedrgbcolors[0]-detectedrgbcolors[lowestval_idx]));
        }
        Serial.print("  ");
        if (detectedrgbcolors[1]-detectedrgbcolors[lowestval_idx] < detectedrgbcolors[2]-detectedrgbcolors[lowestval_idx]) {
          Serial.print((float)(detectedrgbcolors[1]-detectedrgbcolors[lowestval_idx])/(float)(detectedrgbcolors[2]-detectedrgbcolors[lowestval_idx]));
        } else {
          Serial.print((float)(detectedrgbcolors[2]-detectedrgbcolors[lowestval_idx])/(float)(detectedrgbcolors[1]-detectedrgbcolors[lowestval_idx]));
        }
        Serial.print("  ");
        if (detectedrgbcolors[2]-detectedrgbcolors[lowestval_idx] < detectedrgbcolors[0]-detectedrgbcolors[lowestval_idx]) {
          Serial.println((float)(detectedrgbcolors[2]-detectedrgbcolors[lowestval_idx])/(float)(detectedrgbcolors[0]-detectedrgbcolors[lowestval_idx]));
        } else {
          Serial.println((float)(detectedrgbcolors[0]-detectedrgbcolors[lowestval_idx])/(float)(detectedrgbcolors[2]-detectedrgbcolors[lowestval_idx]));
        }
        */
        startTimer(waitcolorreadings, t);
        colorsamples++;
      } else {
        // reset sample count and stop wheel
        colorsamples = 0;
        //Serial.println("Feed Wheel: All samples collected");
        Serial.println("");

        state = feeditem;
      }
    }
    break;
  case showcalibration:
    // calibration done, stop wheel
    //Serial.println("Feed Wheel: Calibration DONE!");
    
    // print array code to be copied and pasted into colors.ino
    Serial.println("");
    Serial.println("Feed Wheel: Add the following array to colorsensor.ino:");
    Serial.println("double calibratedhsv[7][2][3] = {");
    for(i = 0; i < 7; i++)
    {
      colorhue[i] = 0; colorsat[i] = 0; colorval[i] = 0;
      colorsumhue[i] = 0; colorsumsat[i] = 0; colorsumval[i] = 0;
      
      // rounds
      for(k = 0; k < 2; k++)
      {
        // samples
        for(j = 0; j < 3; j++)
        {          
          // std deviation - addition
          rgbToHSV((double)calibratedcolors[k][i][j][0], (double)calibratedcolors[k][i][j][1], (double)calibratedcolors[k][i][j][2], hsv);
          if (hsv[0] > 356) hsv[0] = 0;
          colorhue[i] += hsv[0];
          colorsat[i] += hsv[1];
          colorval[i] += hsv[2];
        }
      }
      Serial.print("  { // item ");
      Serial.println(colortext[i]);

      /*
      Serial.print("Sum for ");
      Serial.print(colortext[i]);
      Serial.print(": ");
      Serial.print(colorhue[i]);
      Serial.print(" ");
      Serial.print(colorsat[i]);
      Serial.print(" ");
      Serial.println(colorval[i]);
      */
      
      colorhue[i] /= (double)6;
      colorsat[i] /= (double)6;
      colorval[i] /= (double)6;
      /*
      Serial.print("Mean for ");
      Serial.print(colortext[i]);
      Serial.print(": ");
      Serial.print(colorhue[i]);
      Serial.print(" ");
      Serial.print(colorsat[i]);
      Serial.print(" ");
      Serial.println(colorval[i]);
      */

      Serial.print("    {");
      Serial.print(colorhue[i]);
      Serial.print(",");
      Serial.print(colorsat[i]);
      Serial.print(",");
      Serial.print(colorval[i]);
      Serial.println("}, // mean");
      
      // rounds
      for(k = 0; k < 2; k++)
      {
        // samples
        for(j = 0; j < 3; j++)
        {
          rgbToHSV((double)calibratedcolors[k][i][j][0], (double)calibratedcolors[k][i][j][1], (double)calibratedcolors[k][i][j][2], hsv);
          if (hsv[0] > 356) hsv[0] = 0;
          colorsumhue[i] += pow(hsv[0]-colorhue[i], 2);
          colorsumsat[i] += pow(hsv[1]-colorsat[i], 2);
          colorsumval[i] += pow(hsv[2]-colorval[i], 2);
        }
      }
      colorsumhue[i] = sqrt(colorsumhue[i]/6);
      colorsumsat[i] = sqrt(colorsumsat[i]/6);
      colorsumval[i] = sqrt(colorsumval[i]/6);
      /*
      Serial.print("Population standard derivation for ");
      Serial.print(colortext[i]);
      Serial.print(": ");      
      Serial.print(colorsumhue[i]);
      Serial.print(" ");
      Serial.print(colorsumsat[i]);
      Serial.print(" ");
      Serial.println(colorsumval[i]);
      */

      Serial.print("    {");
      Serial.print(colorsumhue[i]);
      Serial.print(",");
      Serial.print(colorsumsat[i]);
      Serial.print(",");
      Serial.print(colorsumval[i]);
      Serial.println("}, // stddev");

      Serial.println("  },");

    }
    Serial.println("};");
    
    state = donecalibration;
    break;
  case donecalibration:
    // finished
    Serial.println("");
    Serial.println("To restart, type R");
    while ((char)Serial.read() != 'R');
    state = feeditem;

    break;
  }
}

