#include "cam.h"
#include "web.h"

void setup() {
  Serial.begin(115200);
  delay(2000);

  if (!initCamera()) {
    Serial.println("Camera init failed");
    while (true);
  }

  startServer();
  Serial.println("Server started");
}

void loop() {
  detectMotion();
}