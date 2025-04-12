#include "cam.h"
#include "esp_system.h"  // psramFound
#include "Arduino.h"     // delay
#include <cstring>       // memcpy
#include <cstdlib>       // abs

#define CAM_WIDTH 160
#define CAM_HEIGHT 120
#define MOTION_THRESHOLD 25
#define CHANGE_LIMIT 1000

static uint8_t previousFrame[CAM_WIDTH * CAM_HEIGHT];
static int lastMotionScore = 0;
static camera_fb_t* lastCapturedFrame = nullptr;

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

    if (psramFound()) {
        config.frame_size = FRAMESIZE_QQVGA;
        config.jpeg_quality = 12;
        config.fb_count = 2;
        config.fb_location = CAMERA_FB_IN_PSRAM;
    } else {
        return false;
    }

    return esp_camera_init(&config) == ESP_OK;
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

    memcpy(previousFrame, fb->buf, fb->len);
    esp_camera_fb_return(fb);
    lastMotionScore = diffCount;

    if (diffCount > CHANGE_LIMIT) {
        if (lastCapturedFrame != nullptr) {
            esp_camera_fb_return(lastCapturedFrame);
            lastCapturedFrame = nullptr;
        }

        sensor_t* s = esp_camera_sensor_get();
        s->set_pixformat(s, PIXFORMAT_JPEG);
        s->set_framesize(s, FRAMESIZE_QVGA);
        delay(100);
        lastCapturedFrame = esp_camera_fb_get();

        sensor_t* s2 = esp_camera_sensor_get();
        s2->set_pixformat(s2, PIXFORMAT_GRAYSCALE);
        s2->set_framesize(s2, FRAMESIZE_QQVGA);
        delay(100);

        return true;
    }

    return false;
}

int getLastMotionScore() {
    return lastMotionScore;
}

camera_fb_t* captureSnapshot() {
    return lastCapturedFrame;
}