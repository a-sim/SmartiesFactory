//
// COLOR LIGHT SENSOR DECLARE aka. colorsensor.c
//

#include "i2c.h"
#include "colorsensor.h"

void colorCalibrate(void)
{
  // Turn on LED
  digitalWrite(LEDLIGHT_PIN, HIGH);

  // Wait for ADJD reset sequence
  delay(1);

  // Sensor gain setting (Avago app note 5330) - Calibrated for primary color detection (RGB).
  // CAPs are 4bit (higher value will result in lower output)
  // Valid values from 0 - 15 decimal
  i2c_writebyte(I2C_ADDR, REG_CAP_RED, 0x0E); // 0x08
  i2c_writebyte(I2C_ADDR, REG_CAP_GREEN, 0x0F); // 0x0C
  i2c_writebyte(I2C_ADDR, REG_CAP_BLUE, 0x06); // 0x05
  i2c_writebyte(I2C_ADDR, REG_CAP_CLEAR, 0x0F); // 0x0A
  
  // Integration time slot values are 12bit (higher value will result in higher sensor value)
  // Valid values from 0 - 4096 decimal
  i2c_writebyte(I2C_ADDR, REG_INT_RED_LO, 0x2C);
  i2c_writebyte(I2C_ADDR, REG_INT_RED_HI, 0x01);
  i2c_writebyte(I2C_ADDR, REG_INT_GREEN_LO, 0x2C);
  i2c_writebyte(I2C_ADDR, REG_INT_GREEN_HI, 0x01);
  i2c_writebyte(I2C_ADDR, REG_INT_BLUE_LO, 0x2C);
  i2c_writebyte(I2C_ADDR, REG_INT_BLUE_HI, 0x01);
  i2c_writebyte(I2C_ADDR, REG_INT_CLEAR_LO, 0xC8);
  i2c_writebyte(I2C_ADDR, REG_INT_CLEAR_HI, 0x00);
}

void getColors(int colors[])
{
  static char init = 0;
  unsigned int r, r_lo, r_hi, g, g_lo, g_hi, b, b_lo, b_hi, c, c_lo, c_hi;
  unsigned int v;

  if (!init) {
    // Port setup, outputs and pull-up on SDA, SCL, and LED-pin
    pinMode(LEDLIGHT_PIN, OUTPUT); pinMode(SDA_PIN, OUTPUT); pinMode(SCL_PIN, OUTPUT); 
    digitalWrite(LEDLIGHT_PIN, HIGH); digitalWrite(SDA_PIN, HIGH); digitalWrite(SCL_PIN, HIGH);
    
    // Initilize i2c and sensor caps/integration values
    i2c_init();
    colorCalibrate();
  
    init = 1;
  }

  // Turn on LED
  digitalWrite(LEDLIGHT_PIN, HIGH);

  // Wait for LEDs to light up properly
  delay(100);

  // Start color light sensing  
  i2c_writebyte(I2C_ADDR,0x00,0x01);
  
  while(i2c_readbyte(I2C_ADDR,0x00,&v) && v != 0) {
  // Waiting for a result, CTRL register bit will be set to 1 when the result is ready
  }

  // RGB color results, 10bit results
  i2c_readbyte(I2C_ADDR,REG_DATA_RED_LO,&r_lo);
  i2c_readbyte(I2C_ADDR,REG_DATA_RED_HI,&r_hi);
  r = r_lo + (r_hi<<8);
  i2c_readbyte(I2C_ADDR,REG_DATA_GREEN_LO,&g_lo);
  i2c_readbyte(I2C_ADDR,REG_DATA_GREEN_HI,&g_hi);
  g = g_lo + (g_hi<<8);
  i2c_readbyte(I2C_ADDR,REG_DATA_BLUE_LO,&b_lo);
  i2c_readbyte(I2C_ADDR,REG_DATA_BLUE_HI,&b_hi);
  b = b_lo + (b_hi<<8);
  i2c_readbyte(I2C_ADDR,REG_DATA_CLEAR_LO,&c_lo);
  i2c_readbyte(I2C_ADDR,REG_DATA_CLEAR_HI,&c_hi);
  c = c_lo + (c_hi<<8);

  // Put values into existing passed array 
  colors[0] = r;
  colors[1] = g;
  colors[2] = b;
  colors[3] = c;

  // Turn off LED
  digitalWrite(LEDLIGHT_PIN, LOW);
}

ItemColor determineColor(int detectedcolors[])
{
  int i, j, maxhits = 0;
  int colorstats[7] = {0, 0, 0, 0, 0, 0, 0};
  const static int standdev = 2*30;
  ItemColor itemcolor = unknown;
  
  // Calibrated values from the actual items (M&Ms or Skittles)
int calibratedcolors[7][3][3] = {
  { // item unknown
    {0,0,0},
    {0,0,0},
    {0,0,0},
  },
  { // item red
    {0,0,0},
    {0,0,0},
    {0,0,0},
  },
  { // item green
    {0,0,0},
    {0,0,0},
    {0,0,0},
  },
  { // item blue
    {0,0,0},
    {0,0,0},
    {0,0,0},
  },
  { // item orange
    {0,0,0},
    {0,0,0},
    {0,0,0},
  },
  { // item yellow
    {0,0,0},
    {0,0,0},
    {0,0,0},
  },
  { // item purple
    {0,0,0},
    {0,0,0},
    {0,0,0},
  },
};

  // Find color - based on 2 times standard derivation of the RGB values
  for(i = 1; i < 7; i++)
  {
      for(j = 0; j < 6; j++)
      {
        if (calibratedcolors[i][j][0] > (detectedcolors[0]-standdev) && calibratedcolors[i][j][0] < (detectedcolors[0]+standdev)
          && calibratedcolors[i][j][1] > (detectedcolors[1]-standdev) && calibratedcolors[i][j][1] < (detectedcolors[1]+standdev)
          && calibratedcolors[i][j][2] > (detectedcolors[2]-standdev) && calibratedcolors[i][j][2] < (detectedcolors[2]+standdev))
        {
          colorstats[i]++;
        }
      }
  }
  
  // Find the color with the most hits
  for(i = 1; i < 7; i++)
  {
    if (colorstats[i] > maxhits) {
      maxhits = colorstats[i];
      itemcolor = (ItemColor)i;
    }
  }
  return itemcolor;
}

void rgbToHSV(double r, double g, double b, double hsv[]) {
  double computedH, computedS, computedV;
 double minRGB, maxRGB;
 double d = 0, h = 0;

  r = r/1023; g = g/1023; b = b/1023;
  minRGB = min(r,min(g,b));
  maxRGB = max(r,max(g,b));

  if (minRGB == maxRGB) {
    // Black-gray-white
    computedV = minRGB;
    hsv[0] = 0; hsv[1] = 0; hsv[2] = computedV;
  } else { 
    // Colors other than black-gray-white:
    d = (r == minRGB) ? g-b : ((b == minRGB) ? r-g : b-r);
    h = (r == minRGB) ? 3 : ((b == minRGB) ? 1 : 5);
  
    computedH = 60*(h - (d/(maxRGB - minRGB)));
    computedS = (maxRGB - minRGB)/maxRGB;
    computedV = maxRGB;
    
    hsv[0] = computedH; hsv[1] = computedS; hsv[2] = computedV;
  }
}
