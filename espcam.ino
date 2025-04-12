#include "cam.h"

#include <WiFi.h>
#include "esp_camera.h"

const char* ssid = "TOPNET_VSKC";
const char* password = "a47qhmlwxy";

void setup(){
  Serial.begin(115200);
  initCamera();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  
  startCameraWebServer();
}
void loop(){}