// cam.h
#ifndef CAM_H
#define CAM_H

#include "esp_camera.h"

// ---- CAMERA SETUP ----
bool initCamera();

// ---- MOTION DETECTION ----
// returns true if motion is detected and prints it to Serial
bool detectMotion();

// Optionally get the last detection value
int getLastMotionScore();

#endif
