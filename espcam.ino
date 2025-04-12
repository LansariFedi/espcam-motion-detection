#include "cam.h"

void setup() {
  Serial.begin(115200);
  delay(2000);

  if (!initCamera()) {
    Serial.println("Camera init failed!");
    while (1);
  }
  Serial.println("Camera initialized. Starting motion detection...");
}

void loop() {
  detectMotion(); 
  delay(500);     
}
