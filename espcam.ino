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
  detectMotion();  // will print to Serial when motion is detected
  delay(500);      // change this delay for faster/slower checks
}
