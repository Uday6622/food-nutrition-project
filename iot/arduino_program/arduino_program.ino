void setup() {
  // Initialize hardware serial
  Serial.begin(115200);      // For Serial Monitor
  Serial1.begin(115200);     // For communication with ESP32
  
  Serial.println("Arduino ready to communicate with ESP32...");
}

void loop() {
  // Send command to ESP32 to trigger image capture
  Serial.println("Sending capture command to ESP32...");
  Serial1.println("CAPTURE");  // Send the "CAPTURE" command via Serial1
  
  // Wait for ESP32 response
  while (Serial1.available()) {
    String response = Serial1.readStringUntil('\n');
    Serial.print("Response from ESP32: ");
    Serial.println(response);  // Print the response from ESP32
  }

  delay(10000);  // Wait 10 seconds before sending the next capture command
}
