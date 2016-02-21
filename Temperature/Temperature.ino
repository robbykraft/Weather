#include <ArduinoJson.h>

#include <Bridge.h>
#include <HttpClient.h>

void setup() {
  Bridge.begin();
  Console.begin();  
  while(!Console);
  Console.println("Console connected!");

//  runCurl();
}


void loop() {
  Console.println("In loop");
//  runCurl();  
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

//  char response[600];
  char *response = (char*)malloc(sizeof(char)*600);
  processResponse(&p, response);

  Console.println("Response array");
  Console.println(response);

  free(response);
  Console.flush();
}

void processResponse(Process* proc, char* response) {
  int i = 0;
  while(proc->available() > 0) {
    char c = proc->read();
    response[i] = c;
    i++;
  }
  response[i] = '\0';
}

