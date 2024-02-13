#include <WiFi.h>

const char *ssid = "HARAJIT";
const char *password = "HARAJIT05";

const int externalLedPin = 5; // Replace with the GPIO pin connected to your external LED
const int builtInLedPin = 2;   // Built-in LED pin on ESP32

WiFiServer server(80);

void setup() {
  pinMode(externalLedPin, OUTPUT);
  pinMode(builtInLedPin, OUTPUT);

  // Start Serial for debugging purposes
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi network with IP: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    // Wait for a request
    while (!client.available()) {
      delay(1);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);

    // Check if the request is for turning on or off the LED
    if (request.indexOf("/on") != -1) {
      digitalWrite(externalLedPin, HIGH);
      digitalWrite(builtInLedPin, HIGH); // Turn on built-in LED
    } else if (request.indexOf("/off") != -1) {
      digitalWrite(externalLedPin, LOW);
      digitalWrite(builtInLedPin, LOW); // Turn off built-in LED
    }

    // Send the response to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");

    // Header
    client.println("<head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<style>");
    client.println("body { font-family: Arial, Helvetica, sans-serif; }");
    client.println(".header { background-color: #f1f1f1; padding: 20px; text-align: center; }");
    client.println(".footer { background-color: #f1f1f1; padding: 10px; text-align: center; }");
    client.println(".button { display: inline-block; padding: 10px 20px; font-size: 16px; margin: 10px; }");
    client.println(".on { background-color: #4CAF50; color: white; }");
    client.println(".off { background-color: #f44336; color: white; }");
    client.println("</style>");
    client.println("</head>");

    // Body
    client.println("<body>");

    // Header
    client.println("<div class='header'>");
    client.println("<h2>LED Control</h2>");
    client.println("</div>");

    // Output the LED control buttons
    client.println("<p>External LED Control:</p>");
    client.println("<a href='/on'><button class='button on'>ON</button></a>&nbsp;");
    client.println("<a href='/off'><button class='button off'>OFF</button></a>");

    // Footer
    client.println("<div class='footer'>");
    client.println("<p>ESP32 Access Point - " + WiFi.localIP().toString() + "</p>");
    client.println("</div>");

    client.println("</body>");
    client.println("</html>");

    // Close the connection
    client.stop();
  }
}
