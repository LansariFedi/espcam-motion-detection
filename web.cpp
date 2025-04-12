#include "web.h"
#include "cam.h"
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

WebServer server(80);

void handleRoot() {
    String html = "<html><head><meta http-equiv='refresh' content='1'/>"
                  "<title>ESP32-CAM Motion</title></head><body>"
                  "<h2>Latest Motion Snapshot:</h2>"
                  "<img src='/snapshot' width='320' height='240'/></body></html>";
    server.send(200, "text/html", html);
}

void handleSnapshot() {
    camera_fb_t* fb = captureSnapshot();

    if (fb && fb->format == PIXFORMAT_JPEG) {
        server.sendHeader("Content-Type", "image/jpeg");
        server.sendHeader("Content-Length", String(fb->len));
        server.send(200);
        WiFiClient client = server.client();
        client.write(fb->buf, fb->len);
    } else {
        server.send(503, "text/plain", "No image yet");
    }
}

void startServer() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/snapshot", HTTP_GET, handleSnapshot);

    server.begin();
}