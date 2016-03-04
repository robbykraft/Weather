#include <ArduinoJson.h>
#include <Bridge.h>
#include "dualSegmentDisplay.h"
#include <Adafruit_NeoPixel.h>

// time in milliseconds
#define THREE_MINUTES 180000
#define TEN_MINUTES 600000
#define THIRTY_MINUTES 1800000

#define NUMPIXELS 24
#define PIXELS_PIN 13
enum{
  CLEAR,
  CLOUDS,
  RAIN
};

unsigned long lastReadTime;
DualSegmentDisplay dualSegment;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

float hourlyTemp[24];
int hourlyWeather[24];

void setup() {
  Bridge.begin();
//  Console.begin();
//  while (!Console);
//  Console.println("Console connected!");
  setupPixels();
  for(int i = 0; i < 24; i++){
    hourlyTemp[i] = 0;
    hourlyWeather[i] = 0;
  }

  dualSegment.setup();
  // delay first read, wait for WIFI to connect
  lastReadTime = millis() - TEN_MINUTES + THREE_MINUTES;  
}

void loop() {
  dualSegment.loop();

  if(millis() > lastReadTime + TEN_MINUTES){  
    dualSegment.setNumber( runCurl() );
    runCurl2();
    setPixels(hourlyTemp);
    lastReadTime = millis();
  }
}

void setPixels(float hours[24]){
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,30,0));
//  for(int i = 1; i < NUMPIXELS; i++){
//    if(hours[i] == CLEAR)
//      pixels.setPixelColor(i, pixels.Color(10,10,10));
//    else if(hours[i] == CLOUDS)
//      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
//    else if(hours[i] == RAIN)
//      pixels.setPixelColor(i, pixels.Color(0,0,5));
//  }
  for(int i = 1; i < NUMPIXELS; i++){
    if(hourlyTemp[i] > hourlyTemp[0]){
      float diff = hourlyTemp[i] - hourlyTemp[0];
      diff *= 3;
      diff += 1;
      if(diff > 255) diff = 255;
      pixels.setPixelColor(i, pixels.Color(diff,0,0));
    }
    else{
      float diff = hourlyTemp[0] - hourlyTemp[i];
      diff *= 3;
      diff += 1;
      if(diff > 255) diff = 255;
      pixels.setPixelColor(i, pixels.Color(0,0,diff));
    }
  }
  pixels.show();
}

void setupPixels(){
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,30,0));
  for(int i = 1; i < 12; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,10));
  for(int i = 15; i < 24; i++)
    pixels.setPixelColor(i, pixels.Color(10,0,0));  
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
  
//  char *response = (char*)malloc(sizeof(char) * 1600);
  char *response = (char*)malloc(sizeof(char) * 600);
  processResponse(&p, response);
  double temp = getTempFromResponse(response);
//  getHourlyFromResponse(response);
  Console.println("Temperature");
  Console.println(temp);
  
  free(response);
  Console.flush();

  return temp;
}

void runCurl2() {
  Console.println("Running curl!");
  Process p;
  p.begin("curl");
  p.addParameter("-k");
  p.addParameter("--globoff");
  p.addParameter("http://robbykraft.com/weather.json");
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
  
  char *response = (char*)malloc(sizeof(char) * 650);
  processResponse(&p, response);
  getHourlyFromResponse(response);
  Console.println("Temperature");
  
  free(response);
  Console.flush();
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

void getHourlyFromResponse(char* response){
  StaticJsonBuffer<650> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(response);
    if (!root.success()) {
      Console.println("Failed!");
      return;
  }
  for(int i = 0; i < 24; i++){
    double temp = root["data"][i]["temperature"];
//    double precip = root["data"][i]["precipProbability"];
    hourlyTemp[i] = temp;
  }
//  "precipIntensity":0,
//  "precipProbability":0,
//  "temperature":35.01,
}
/*
void getHourlyFromResponse(char* response){
  StaticJsonBuffer<1150> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(response);
    if (!root.success()) {
      Console.println("Failed!");
      return;
  }
  JsonObject& hourly = root["hourly"];
  if (!hourly.success()) {
    Console.println("Hourly failed!");
    return;
  }
  for(int i = 0; i < 24; i++){
    double temp = hourly["data"][i]["temperature"];
    double precip = hourly["data"][i]["precipProbability"];
    hourlyTemp[i] = temp;
  }
//  "precipIntensity":0,
//  "precipProbability":0,
//  "temperature":35.01,
}
*/
void processResponse(Process* proc, char* response) {
  int i = 0;
  while (proc->available() > 0) {
    char c = proc->read();
    response[i] = c;
    i++;
  }
  response[i] = '\0';
}

