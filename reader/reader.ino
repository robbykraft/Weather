#include <ArduinoJson.h>
#include <Bridge.h>
#include "dualSegmentDisplay.h"
#include <Adafruit_NeoPixel.h>
#define ONE_MINUTE 60000
#define THREE_MINUTES 180000
#define TEN_MINUTES 600000
#define THIRTY_MINUTES 1800000


#define READ_INTERVAL TEN_MINUTES  // interval between API fetches
unsigned long lastReadTime;
#define ANIMATION_INTERVAL 30
unsigned long lastAnimTime;
#define NUMPIXELS 24
#define PIXELS_PIN 13


DualSegmentDisplay dualSegment;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);
// saved weather data
float hourlyTemp[24];
float hourlyPrecip[24];
// convert weather data and store interpretation
uint8_t pColor[24][3];
uint8_t pAnimMod[24];

void setup() {
  Bridge.begin();
//  Console.begin();
//  while (!Console);
//  Console.println("Console connected!");
  for(int i = 0; i < 24; i++){
    hourlyTemp[i] = 0;
    hourlyPrecip[i] = 0;
    pAnimMod[i] = random(40)+20;
  }
  setupPixels(); 
  dualSegment.setup();
  // delay first read to wait for WIFI to connect
  //   (if hard boot, needs about three minutes to connect)
  lastReadTime = (millis()-READ_INTERVAL) + 10000;//THREE_MINUTES;  
}

void loop() {
  dualSegment.loop();
  if(millis() > lastReadTime + READ_INTERVAL){
    getHourlyFromBridge();
    dualSegment.setInteger( hourlyTemp[0] );
    interpretWeatherData();
    lastReadTime = millis();
  }
  if(millis() > lastAnimTime + ANIMATION_INTERVAL){
    updatePixels();
    lastAnimTime = millis();
  }
}

//////////////////////////////////
/////////    FETCH DATA

void getHourlyFromBridge(){
  Process p;
  p.runShellCommand("python /mnt/sda1/arduino/BridgeClient.py");
  char value[5];
  for(int i = 0; i < 24; i++){
    // make strings "D0" - "D23"
    Bridge.get(String("D" + String(i)).c_str(), value, 5);
    hourlyTemp[i] = atof(value);
    Bridge.get(String("P" + String(i)).c_str(), value, 5);
    hourlyPrecip[i] = atof(value);
    pAnimMod[i] = random(40)+20;
  }
}

void interpretWeatherData(){
  for(int i = 1; i < NUMPIXELS; i++){
    if(hourlyTemp[i] > hourlyTemp[0]){
      float diff = hourlyTemp[i] - hourlyTemp[0];
      diff = sqrt(diff * 16);
      if(diff > 50) diff = 50;
      pColor[i][0] = diff;
      pColor[i][1] = diff;
      pColor[i][2] = 0;
    }
    else if(hourlyTemp[i] < hourlyTemp[0]){
      float diff = hourlyTemp[0] - hourlyTemp[i];
      diff = sqrt(diff * 16);
      if(diff > 50) diff = 50;
      pColor[i][0] = diff*.3;
      pColor[i][1] = diff*.3;
      pColor[i][2] = diff;
    }
    else{
      pColor[i][0] = 0;
      pColor[i][1] = 0;
      pColor[i][2] = 0;
    }
  }
}

//////////////////////////////////
/////////    NEOPIXELS

void setupPixels(){
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(30,0,0));
  for(int i = 1; i < 24; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0));  
  pixels.show();
}

void updatePixels(){
  static unsigned long bigCount = 0;
  bigCount++; 
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(30,0,0));
  for(int i = 1; i < NUMPIXELS; i++){
    if(hourlyPrecip[i] == 0)
      pixels.setPixelColor(i, pixels.Color(pColor[i][0], pColor[i][1], pColor[i][2]) );
    else{
      float anim = 1.0 - ((bigCount%pAnimMod[i])/(float)pAnimMod[i])*hourlyPrecip[i];
      pixels.setPixelColor(i, pixels.Color(pColor[i][0]*anim, pColor[i][1]*anim, pColor[i][2]*anim) );
    }
  }
  pixels.show();  
}
