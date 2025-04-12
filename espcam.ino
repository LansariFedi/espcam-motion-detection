#include "esp_camera.h"
#include <WiFi.h>

#include "cam.h"

const char* ssid = "TOPNET_VSKC";
const char* password = "a47qhmlwxy";

void startCameraServer();
void setupLedFlash(int pin);


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  initCamera();
  //startCameraServer();

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
}
