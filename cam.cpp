#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "esp_heap_caps.h"
#include "camera_pins.h"
#include "Arduino.h"
#include <esp_timer.h>

void startCameraServer();
void setupLedFlash(int pin);

camera_fb_t *previous_frame = nullptr;
unsigned long last_motion_time = 0; // Timestamp of the last detected motion
const unsigned long motion_cooldown = 2000; // Cooldown period in milliseconds

void initCamera() {
  camera_config_t config;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif
}

bool detectMotion(camera_fb_t *current_frame, camera_fb_t *previous_frame) {
  if (!current_frame || !previous_frame) {
    return false;
  }

  if (current_frame->len != previous_frame->len) {
    return true;
  }

  int diff_count = 0;
  for (size_t i = 0; i < current_frame->len; i++) {
    if (current_frame->buf[i] != previous_frame->buf[i]) {
      diff_count++;
    }

    if (diff_count > 2000) { // Increased threshold for motion detection
      return true;
    }
  }

  return false;
}

void captureAndDetectMotion() {
  camera_fb_t *current_frame = esp_camera_fb_get();
  if (!current_frame) {
    Serial.println("Failed to capture frame");
    return;
  }

  unsigned long current_time = millis();
  bool motion_detected = detectMotion(current_frame, previous_frame);

  if (motion_detected && (current_time - last_motion_time > motion_cooldown)) {
    Serial.println("Motion detected!");
    last_motion_time = current_time; // Update the last motion timestamp
  }

  if (previous_frame) {
    esp_camera_fb_return(previous_frame);
  }
  previous_frame = current_frame;
}