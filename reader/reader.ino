#include <ArduinoJson.h>
#include <Bridge.h>
#include "dualSegmentDisplay.h"
#include <Adafruit_NeoPixel.h>
#define ONE_MINUTE 60000
#define THREE_MINUTES 180000
#define TEN_MINUTES 600000
#define THIRTY_MINUTES 1800000


#define READ_INTERVAL TEN_MINUTES  // interval between API fetches
#define NUMPIXELS 24
#define PIXELS_PIN 13


unsigned long lastReadTime;
DualSegmentDisplay dualSegment;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);
float hourlyTemp[24];  // saved weather data

void setup() {
  Bridge.begin();
//  Console.begin();
//  while (!Console);
//  Console.println("Console connected!");
  for(int i = 0; i < 24; i++)
    hourlyTemp[i] = 0;
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
    dualSegment.setNumber( hourlyTemp[0] );
    updatePixels();
    lastReadTime = millis();
  }
}

//////////////////////////////////
/////////    NEOPIXELS

void setupPixels(){
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,30,0));
  for(int i = 1; i < 24; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0));  
  pixels.show();
}

void updatePixels(){
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,30,0));
  
  for(int i = 1; i < NUMPIXELS; i++){
    if(hourlyTemp[i] > hourlyTemp[0]){
      float diff = hourlyTemp[i] - hourlyTemp[0];
      diff = sqrt(diff * 16);
      if(diff > 50) diff = 50;
      pixels.setPixelColor(i, pixels.Color(diff,0,0));
    }
    else if(hourlyTemp[i] < hourlyTemp[0]){
      float diff = hourlyTemp[0] - hourlyTemp[i];
      diff = sqrt(diff * 16);
      if(diff > 50) diff = 50;
      pixels.setPixelColor(i, pixels.Color(0,0,diff));
    }
    else{
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
  }
  pixels.show();
}

//////////////////////////////////
/////////    FETCH DATA

void getHourlyFromBridge(){
  Process p;
  p.runShellCommand("python /mnt/sda1/arduino/BridgeClient.py");
  char Dvalue[5];
  for(int i = 0; i < 24; i++){
    // make strings "D0" - "D23"
    Bridge.get(String("D" + String(i)).c_str(), Dvalue, 5);
    hourlyTemp[i] = atof(Dvalue);
  }
}
