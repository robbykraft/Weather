#include <ArduinoJson.h>
#include <Bridge.h>
#include "dualSegmentDisplay.h"
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 24
#define PIXELS_PIN 13

// time in milliseconds
#define ONE_MINUTE 60000
#define THREE_MINUTES 180000
#define TEN_MINUTES 600000
#define THIRTY_MINUTES 1800000

// SET HERE: interval between API fetch
#define READ_INTERVAL TEN_MINUTES

unsigned long lastReadTime;
DualSegmentDisplay dualSegment;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

float hourlyTemp[24];

void setup() {
  Bridge.begin();
//  Console.begin();
//  while (!Console);
//  Console.println("Console connected!");

  for(int i = 0; i < 24; i++)
    hourlyTemp[i] = 0;

  setupPixels(); 
  dualSegment.setup();
  // delay first read, wait for WIFI to connect
  lastReadTime = (millis()-READ_INTERVAL) + THREE_MINUTES;  
}

void loop() {
  dualSegment.loop();
  if(millis() > lastReadTime + READ_INTERVAL){  
    getHourlyFromBridge();
    dualSegment.setNumber( runCurl() );
    setPixels(hourlyTemp);
    lastReadTime = millis();
  }
}

void setupPixels(){
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,30,0));
  for(int i = 1; i < 24; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0));  
  pixels.show();
}

void setPixels(float hours[24]){
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,30,0));
  
  for(int i = 1; i < NUMPIXELS; i++){
    if(hourlyTemp[i] > hourlyTemp[0]){
      float diff = hourlyTemp[i] - hourlyTemp[0];
      diff *= 3;
      diff += 1;
      if(diff > 50) diff = 50;
      pixels.setPixelColor(i, pixels.Color(diff,0,0));
    }
    else{
      float diff = hourlyTemp[0] - hourlyTemp[i];
      diff *= 3;
      diff += 1;
      if(diff > 50) diff = 50;
      pixels.setPixelColor(i, pixels.Color(0,0,diff));
    }
  }
  pixels.show();
}

double runCurl() {
  Console.println("Running curl!");
  Process p;
  p.begin("curl");
  p.addParameter("-k");
  p.addParameter("--globoff");
  p.addParameter("https://api.forecast.io/forecast/2c3297c78ee5f772a0005a607f7d6a53/40.694,-73.9186?exclude=[minutely,hourly,daily,alerts,flags]");
  p.run();
  //  p.runShellCommand("curl --globoff -k https://api.forecast.io/forecast/2c3297c78ee5f772a0005a607f7d6a53/40.694,-73.9186?exclude=[minutely,hourly,daily,alerts,flags]");

  int exitValue = p.exitValue();
  if (exitValue != 0) {
    Console.println("Exit value was " + exitValue);
  } else {
    Console.println("Success! Exit was 0");
  }

  if (p.available() > 0) {
    Console.println(p.available() + " bytes available");
  } else {
    Console.println("No bytes");
  }
  
  char *response = (char*)malloc(sizeof(char) * 600);
  processResponse(&p, response);
  double temp = getTempFromResponse(response);

  Console.println("Temperature");
  Console.println(temp);
  
  free(response);
  Console.flush();

  return temp;
}

double getTempFromResponse(char* response) {
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(response);
    if (!root.success()) {
      Console.println("Failed!");
      return 0.00;
  }
  JsonObject& current = root["currently"];
  if (!current.success()) {
    Console.println("Current failed!");
    return 0.00;
  }
  double temp = current["temperature"];
  return temp;
}

void processResponse(Process* proc, char* response) {
  int i = 0;
  while (proc->available() > 0) {
    char c = proc->read();
    response[i] = c;
    i++;
  }
  response[i] = '\0';
}

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
