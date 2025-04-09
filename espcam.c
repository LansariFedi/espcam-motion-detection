#include "espcam.h"

// Initialize the camera
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

    // Frame size and quality
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;

    // Initialize the camera
    if (esp_camera_init(&config) != ESP_OK) {
        Serial.println("Camera initialization failed");
        return;
    }
    Serial.println("Camera initialized successfully");
}

// Connect to WiFi
void connectToWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

bool detectMotion() {
  // Capture the first frame
  camera_fb_t *frame1 = esp_camera_fb_get();
  if (!frame1) {
      Serial.println("Failed to capture the first frame");
      return false;
  }

  delay(100); // Small delay between frames

  // Capture the second frame
  camera_fb_t *frame2 = esp_camera_fb_get();
  if (!frame2) {
      Serial.println("Failed to capture the second frame");
      esp_camera_fb_return(frame1);
      return false;
  }

  // Compare the two frames
  int diffCount = 0;
  int threshold = 1000; // Adjust this threshold based on your sensitivity needs

  for (size_t i = 0; i < frame1->len && i < frame2->len; i++) {
      if (abs(frame1->buf[i] - frame2->buf[i]) > 20) { // Pixel difference threshold
          diffCount++;
      }
  }

  // Release the frames
  esp_camera_fb_return(frame1);
  esp_camera_fb_return(frame2);

  // Check if the difference count exceeds the threshold
  if (diffCount > threshold) {
      Serial.println("Motion detected!");
      return true;
  } else {
      Serial.println("No motion detected");
      return false;
  }
}

// Capture and send an image to the webserver
void captureAndSendImage() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    HTTPClient http;
    http.begin(WEBSERVER_URL);
    http.addHeader("Content-Type", "image/jpeg");

    int httpResponseCode = http.POST(fb->buf, fb->len);
    if (httpResponseCode > 0) {
        Serial.printf("Image sent successfully, response code: %d\n", httpResponseCode);
    } else {
        Serial.printf("Failed to send image, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
    esp_camera_fb_return(fb);
}