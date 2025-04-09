#ifndef ESPCAM_H
#define ESPCAM_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <esp_camera.h>
#include <HTTPClient.h>

// Camera configuration for AI Thinker ESP32-CAM
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// WiFi credentials
#define WIFI_SSID "TOPNET_VSKC"
#define WIFI_PASSWORD "a47qhmlwxy"

// Webserver URL
#define WEBSERVER_URL "http://127.0.0.1:5000/image"

// Function prototypes
void initCamera();
void connectToWiFi();
bool detectMotion();
void captureAndSendImage();

#endif // ESPCAM_H