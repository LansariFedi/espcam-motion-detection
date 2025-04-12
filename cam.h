#ifndef CAM_H
#define CAM_H

#include "esp_camera.h"

bool initCamera();
bool detectMotion();
int getLastMotionScore();
camera_fb_t* captureSnapshot();

#endif
