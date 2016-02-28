#include <ArduinoJson.h>
#include <Bridge.h>
#include "dualSegmentDisplay.h"

DualSegmentDisplay dualSegment;

void setup() {
  Bridge.begin();
  Console.begin();
  while (!Console);
  Console.println("Console connected!");

  dualSegment.setup();
  //  runCurl();
}


void loop() {
  //dualSegment.loop();
  Console.println("In loop");
  runCurl();
  delay(5000);
}

void runCurl() {
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

