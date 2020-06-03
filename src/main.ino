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
double splash[24];
unsigned long lastAPICall = 0;
bool switchHigh = false;
// unsigned long intervalAPI = 1000 * 60 * 60 * 1; // 1 hour
unsigned long intervalAPI = 1000 * 60 * 30; // 30 min
// unsigned long intervalAPI = 1000 * 60 * 1; // 1 min
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
  //request.body = "{\"key\":\"value\"}";

  http.get(request, response, headers);
  // Particle.publish("API response", String(response.status));

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
    double thisPrecip = doc[i]["precipProbability"];
    // const char* summary = doc[i]["summary"];
    relativeTemp[i] = thisTemp - temp0;
    if      (thisPrecip > 0.35) { precip[i] = 50; }
    else if (thisPrecip > 0.2)  { precip[i] = 10; }
    else                        { precip[i] = 0; }
  }
  String temp_list = "";
  for (int i = 0; i < 24; i++) {
    temp_list += String(relativeTemp[i]) + ", ";
  }
  // Particle.publish("temperature list", temp_list);
}

void lightRing () {
  float seconds = millis() / 1000.0;
  double hueWave = sin(seconds * 0.1) * 15.0;
  double panelBright = 0.5 + sin(seconds * 0.1 / 3.0) * 0.2;
  // update rain splash effect
  for (int i = 0; i < strip.numPixels(); i++) {
    if (rand() % 1000 < precip[i]) { splash[i] = 0;  }
    if (splash[i] < 1.0) { splash[i] += 0.025; }
    else { splash[i] = 1.0; }
  }
  // set pixel colors
  for (int i = 0; i < strip.numPixels(); i++) {
    byte hue = (relativeTemp[i] > 0) ? hue = 80 + hueWave : hue = 175 + hueWave;
    // float temperatureBright = fabs(relativeTemp[i]);
    // if (temperatureBright > 1.0) { temperatureBright = 1.0; }
    float temperatureBright = fabs(relativeTemp[i]) < 10 ? 0.2 : 1.0;
    float brightness = temperatureBright * splash[i] * panelBright;
    strip.setPixelColor(i, colorHue(hue % 255, brightness));
  }
  // set top pixel
  byte whiteBright = 255 * splash[0] * panelBright * 0.5;
  strip.setPixelColor(0, strip.Color(whiteBright, whiteBright, whiteBright));
  strip.show();
}

void setup () {
  // init everything to 0
  for (int i = 0; i < 24; i++) {
    relativeTemp[i] = 1;
    precip[i] = 0;
    splash[i] = 1;
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
    // for (int i = 0; i < 24; i++) {
    //   strip.setPixelColor(i, strip.Color(255, 255, 255));
    // }
    // strip.show();
    callAPI();
  }
}
