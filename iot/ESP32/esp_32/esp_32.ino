#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "ACT101569122990";
const char* password = "17337036";  // Replace with your Wi-Fi password
const char* serverUrl = "http://127.0.0.1:5000/upload";  // Replace with your Flask server URL

// Camera configuration
camera_config_t config;

void setup() {
  Serial.begin(115200); // For communication with Arduino
  Serial.println("ESP32-CAM ready");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Initialize the camera
  config.pin_d0 = 16;
  config.pin_d1 = 17;
  config.pin_d2 = 18;
  config.pin_d3 = 19;
  config.pin_d4 = 21;
  config.pin_d5 = 22;
  config.pin_d6 = 23;
  config.pin_d7 = 25;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 26;
  config.pin_sscb_sda = 27;
  config.pin_sscb_scl = 28;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera initialization failed!");
    return;
  }
  Serial.println("Camera initialized successfully");
}

void loop() {
  // Check if the Arduino has sent a command
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "CAPTURE") {
      Serial.println("Capturing image...");
      
      // Capture a photo
      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Failed to capture image");
        return;
      }

      // Upload the image to the server
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "multipart/form-data");
      
      int httpResponseCode = http.POST(fb->buf, fb->len);
      if (httpResponseCode == 200) {
        Serial.println("Image uploaded successfully");
      } else {
        Serial.printf("Failed to upload image. HTTP error code: %d\n", httpResponseCode);
      }

      // Release the frame buffer memory
      esp_camera_fb_return(fb);

      // Send response back to Arduino
      Serial.println("UPLOAD_SUCCESS");
    } else {
      Serial.println("Unknown command");
    }
  }
}
