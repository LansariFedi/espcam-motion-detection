#include "espcam.h"

void setup(){
    Serial.begin(115200);
    // Initialize the camera
    initCamera();
    // Connect to WiFi
    connectToWiFi();
}
void loop(){
    // Check for motion
    if (detectMotion()) {

        captureAndSendImage();
    }
    delay(1000);
}
