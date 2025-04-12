// cam.cpp
#include "cam.h"
#include <Arduino.h>  // for Serial and delay

#define CAM_WIDTH 160
#define CAM_HEIGHT 120
#define MOTION_THRESHOLD 25      // Sensitivity per pixel
#define CHANGE_LIMIT 1000        // Number of changed pixels to trigger motion

static uint8_t previousFrame[CAM_WIDTH * CAM_HEIGHT];
static int lastMotionScore = 0;

bool initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 5;
    config.pin_d1 = 18;
    config.pin_d2 = 19;
    config.pin_d3 = 21;
    config.pin_d4 = 36;
    config.pin_d5 = 39;
    config.pin_d6 = 34;
    config.pin_d7 = 35;
    config.pin_xclk = 0;
    config.pin_pclk = 22;
    config.pin_vsync = 25;
    config.pin_href = 23;
    config.pin_sscb_sda = 26;
    config.pin_sscb_scl = 27;
    config.pin_pwdn = 32;
    config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_GRAYSCALE;

    if(psramFound()){
        config.frame_size = FRAMESIZE_QQVGA;
        config.jpeg_quality = 12;
        config.fb_count = 2;
        config.fb_location = CAMERA_FB_IN_PSRAM;
    } else {
        return false;
    }

    esp_err_t err = esp_camera_init(&config);
    return err == ESP_OK;
}

bool detectMotion() {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb || fb->len != CAM_WIDTH * CAM_HEIGHT) {
        esp_camera_fb_return(fb);
        return false;
    }

    int diffCount = 0;
    for (int i = 0; i < fb->len; i++) {
        int diff = abs(fb->buf[i] - previousFrame[i]);
        if (diff > MOTION_THRESHOLD) {
            diffCount++;
            if (diffCount > CHANGE_LIMIT) break;
        }
    }

    // Save current frame for next comparison
    memcpy(previousFrame, fb->buf, fb->len);
    esp_camera_fb_return(fb);

    lastMotionScore = diffCount;

    if (diffCount > CHANGE_LIMIT) {
        Serial.println("⚠️ Motion detected!");
        return true;
    }

    return false;
}

int getLastMotionScore() {
    return lastMotionScore;
}
