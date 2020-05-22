/**
 * Weather clock. MIT open source. Robby Kraft
 * 
 * for the Particle Photon. with:
 * - Adafruit 24 Neopixel ring
 * - Luckylight Dual Alphanumeric display KWA-541XWB-Y
 * - MAX7219 8-digit LED driver
 */

#include <math.h>
#include <ArduinoJson.h>
#include <neopixel.h>
#include "HttpClientLarge.h"
#include "Particle.h"
#include "LedControl.h"

// this needs to match the query string from the proxy server
// todo: fill in the password. removed for security
#define PROXY_QUERY_PASSWORD "/?password="
#define PIXEL_PIN D0
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B
#define SPI_DATA A5
#define SPI_CLOCK A3
#define SPI_CHIP_SELECT A2

// project variables
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
HttpClient http;
LedControl sevenSegment = LedControl(SPI_DATA, SPI_CLOCK, SPI_CHIP_SELECT, 1);

// data from the weather API. initialize to zero
double relativeTemp[24];
double precip[24];
unsigned long lastAPICall = 0;
bool switchHigh = false;
unsigned long intervalAPI = 1000 * 60 * 60 * 1; // 1 hour
unsigned long halfInterval = intervalAPI / 2.0;

// never compare against millis(). only modulus of millis()
bool checkTimer () {
  unsigned long mod = millis() % intervalAPI;
  if (mod < halfInterval && switchHigh == true) {
    switchHigh = false;
    return true;
  }
  if (mod > halfInterval && switchHigh == false) { switchHigh = true; }
  return false;
}

SYSTEM_MODE(AUTOMATIC);

uint32_t colorHue (byte offset, float br) {
  if (offset < 85) {
    return strip.Color(br * (offset * 3), br * (255 - offset * 3), 0);
  } else if (offset < 170) {
    offset -= 85;
    return strip.Color(br * (255 - offset * 3), 0, br * (offset * 3));
  } else {
    offset -= 170;
    return strip.Color(0, br * (offset * 3), br * (255 - offset * 3));
  }
}

void callAPI () {
  http_request_t request;
  http_response_t response;
  http_header_t headers[] = { {"Accept","*/*"}, {NULL,NULL} };

  request.hostname = "weather-clock-proxy.herokuapp.com";
  request.port = 80;
  request.path = PROXY_QUERY_PASSWORD;

  http.get(request, response, headers);
  Particle.publish("API response", String(response.status));

  delay(200);
  StaticJsonDocument<4800> doc;
  DeserializationError error = deserializeJson(doc, response.body.c_str());
  if (error) {
    Particle.publish("JSON parse failed", String(error.c_str()));
    return;
  }
  double temp0 = doc[0]["temperature"];
  
  // update seven segment
  int tens = temp0 / 10.0;
  int ones = ((int)temp0) % 10;
  if (temp0 > 100) { tens = 0; ones = 0; }
  sevenSegment.setDigit(0,1,tens,false);
  sevenSegment.setDigit(0,0,ones,false);
  
  // update data for the neopixel ring
  for (int i = 0; i < 24; i++) {
    double thisTemp = doc[i]["temperature"]; // .as<float>();
    // const char* summary = doc[i]["summary"];
    relativeTemp[i] = thisTemp - temp0;
    precip[i] = doc[i]["precipProbability"];
  }
  String temp_list = "";
  for (int i = 0; i < 24; i++) {
    temp_list += String(relativeTemp[i]) + ", ";
  }
  Particle.publish("temperature list", temp_list);
}

void lightRing () {
  float seconds = millis() / 1000.0;
  double wave = sin(seconds / 2.0) * 15.0;
  for (int i = 0; i < strip.numPixels(); i++) {
    byte hue = (relativeTemp[i] > 0) ? hue = 80 + wave : hue = 175 + wave;
    float brightness = fabs(relativeTemp[i]);
    if (brightness > 1.0) { brightness = 1.0; }
    strip.setPixelColor(i, colorHue(hue % 255, brightness));
  }
  strip.setPixelColor(0, strip.Color(255, 255, 255));
  strip.show();
}

void setup () {
  // init everything to 0
  for (int i = 0; i < 24; i++) {
    relativeTemp[i] = 1;
    precip[i] = 0;
  }
  // reset seven segment
  sevenSegment.shutdown(0,false);
  sevenSegment.setIntensity(0,15);
  sevenSegment.clearDisplay(0);
  // reset neopixel ring
  strip.begin();
  strip.show(); // turn off neopixels
  delay(10000);
  callAPI();
}

void loop () {
  lightRing();
  delay(50);
  if (checkTimer()) {
    for (int i = 0; i < 24; i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
    callAPI();
  }
}
