#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>  // Include the ArduinoJson library

// Replace these with your network credentials
const char* ssid = "James";
const char* password = "necesito_wifi";

// Set up a web server on port 80
WebServer server(80);

// Function to handle the root path "/"
void handleRoot() {
  // Add CORS headers
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "ESP32 is ready to receive values.");
}

// Function to handle the "/set_angle" endpoint
void handleSetAngle() {
  // Add CORS headers
  server.sendHeader("Access-Control-Allow-Origin", "*");
  
  if (server.hasArg("plain")) {
    // Get the raw POST body data
    String body = server.arg("plain");

    // Parse the JSON data from the request body
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    // Check for JSON parsing errors
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      server.send(400, "application/json", "{\"status\":\"error\", \"message\":\"invalid JSON\"}");
      return;
    }

    // Extract the 'angle' value from the parsed JSON
    int angle = doc["angle"];  // Get the 'angle' key from the JSON

    // Print the received angle to the Serial Monitor
    Serial.println("Received angle: " + String(angle));

    // Send a success response
    server.send(200, "application/json", "{\"status\":\"success\"}");
  } else {
    server.send(400, "application/json", "{\"status\":\"error\", \"message\":\"no angle provided\"}");
  }
}

// Handle preflight CORS requests (OPTIONS method)
void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);  // No content needed for the OPTIONS response
}

void setup() {
  Serial.begin(115200);  // Start Serial Monitor

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print the ESP32 IP address when connected to Wi-Fi
  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up routes for the web server
  server.on("/", handleRoot);  // Handle the root "/"
  server.on("/set_angle", HTTP_POST, handleSetAngle);  // Handle "/set_angle" POST requests

  // Handle preflight (CORS) requests
  server.on("/set_angle", HTTP_OPTIONS, handleOptions);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}
