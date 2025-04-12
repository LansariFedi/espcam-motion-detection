#include "cam.h"

#include <WebServer.h>
#include "WiFi.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <FS.h>


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
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



void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;


  config.frame_size = FRAMESIZE_QVGA;   
  config.jpeg_quality = 12;              
  config.fb_count = 1;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  Serial.println("Camera initialized successfully!");
}

void takePicture() {
  camera_fb_t* fb = esp_camera_fb_get();
  
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  Serial.print("Picture taken successfully! Size: ");
  Serial.print(fb->len);
  Serial.println(" bytes");

  esp_camera_fb_return(fb); 
}

void handleJPGStream(AsyncWebServerRequest *request) {
  AsyncClient *client = request->client();
  
  if (!(client && client->connected())) return;

  String response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  client->write(response.c_str());

  while (client->connected()) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      break;
    }

    client->write("--frame\r\n");
    client->write("Content-Type: image/jpeg\r\n\r\n");
    client->write(reinterpret_cast<const char*>(fb->buf), fb->len);
    client->write("\r\n");

    esp_camera_fb_return(fb);
    delay(100);  // Adjust frame rate
  }
}

void startCameraWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", 
      "<html><body><h1>ESP32-CAM Stream</h1><img src=\"/stream\"></body></html>");
  });

  server.on("/stream", HTTP_GET, [](AsyncWebServerRequest *request){
    handleJPGStream(request);
  });

  server.begin();
  Serial.println("Async Camera Web Server started");
}