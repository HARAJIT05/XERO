#include <WiFi.h>
#include <SPIFFS.h>

const char *ssid = "HARAJIT";
const char *password = "HARAJIT05";

const int externalLedPin = 15;
const int builtInLedPin = 2;

WiFiServer server(80);

void setup() {
  pinMode(externalLedPin, OUTPUT);
  pinMode(builtInLedPin, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi network with IP: ");
  Serial.println(WiFi.localIP());

  server.begin();

  if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (!client.available()) {
      delay(1);
    }

    String request = client.readStringUntil('\r');
    Serial.println(request);

    if (request.indexOf("/on") != -1) {
      digitalWrite(externalLedPin, HIGH);
      digitalWrite(builtInLedPin, HIGH);
    } else if (request.indexOf("/off") != -1) {
      digitalWrite(externalLedPin, LOW);
      digitalWrite(builtInLedPin, LOW);
    }

    if (request.indexOf("/") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();

    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<style>");
    client.println("body { ");
    client.println("font-family: Arial, Helvetica, sans-serif; ");
    client.println("background-color: #f0f0f0; ");
    client.println("margin: 0; ");
    client.println("padding: 0; ");
    client.println("} ");
    client.println(".container { ");
    client.println("max-width: 600px; ");
    client.println("margin: 0 auto; /* Center horizontally */ ");
    client.println("padding: 20px; ");
    client.println("background-color: #fff; ");
    client.println("border-radius: 10px; ");
    client.println("box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); ");
    client.println("} ");
    client.println(".header { ");
    client.println("background-color: #4CAF50; ");
    client.println("padding: 20px; ");
    client.println("text-align: center; ");
    client.println("color: white; ");
    client.println("border-top-left-radius: 10px; ");
    client.println("border-top-right-radius: 10px; ");
    client.println("} ");
    client.println(".footer { ");
    client.println("background-color: #4CAF50; ");
    client.println("padding: 10px; ");
    client.println("text-align: center; ");
    client.println("color: white; ");
    client.println("border-bottom-left-radius: 10px; ");
    client.println("border-bottom-right-radius: 10px; ");
    client.println("} ");
    client.println(".switch-container { ");
    client.println("display: flex; ");
    client.println("justify-content: center; ");
    client.println("} ");
    client.println(".switch { ");
    client.println("position: relative; ");
    client.println("display: inline-block; ");
    client.println("width: 80px; ");
    client.println("height: 80px; ");
client.println("border: 2px solid #dcdcdc; ");
client.println("background: #e0e0e0; ");
client.println("box-shadow: 7px 7px 23px #bebebe, -7px -7px 23px #ffffff; ");
client.println("overflow: hidden; ");
client.println("border-radius: 60px; ");
client.println("} ");
client.println(".switch input { ");
client.println("opacity: 0; ");
client.println("width: 0; ");
client.println("height: 0; ");
client.println("} ");
client.println(".slider { ");
client.println("position: absolute; ");
client.println("cursor: pointer; ");
client.println("top: 0; ");
client.println("left: 0; ");
client.println("right: 0; ");
client.println("bottom: 0; ");
client.println("-webkit-transition: 0.5s; ");
client.println("transition: 0.5s; ");
client.println("} ");
client.println(".chbox:checked + .slider:before { ");
client.println("background: white; ");
client.println("box-shadow: none; ");
client.println("} ");
client.println(".chbox:focus + .slider { ");
client.println("box-shadow: 0 0 1px #2196f3; ");
client.println("} ");
client.println(".slider { ");
client.println("color: #9a9a9a; ");
client.println("display: flex; ");
client.println("align-items: center; ");
client.println("justify-content: center; ");
client.println("font-size: 11px; ");
client.println("font-family: sans-serif; ");
client.println("} ");
client.println(".slider--0 { ");
client.println("color: white; ");
client.println("font-weight: 600; ");
client.println("background-color: #49d84e; ");
client.println("} ");
client.println(".slider--1 div { ");
client.println("transition: 0.5s; ");
client.println("} ");
client.println(".slider--1 div { ");
client.println("position: absolute; ");
client.println("width: 100%; ");
client.println("height: 50%; ");
client.println("left: 0; ");
client.println("} ");
client.println(".chbox:checked ~ .slider--1 div:first-child { ");
client.println("transform: translateY(-100%); ");
client.println("transition-delay: 1s; ");
client.println("} ");
client.println(".chbox:checked ~ .slider--1 div:last-child { ");
client.println("transform: translateY(100%); ");
client.println("transition-delay: 1s; ");
client.println("} ");
client.println(".chbox:checked ~ .slider--2 { ");
client.println("transform: translateX(100%); ");
client.println("transition-delay: 0.5s; ");
client.println("} ");
client.println(".chbox:checked ~ .slider--3 { ");
client.println("transform: translateX(-100%); ");
client.println("transition-delay: 0s; ");
client.println("} ");
client.println(".slider--1 div:first-child { ");
client.println("transform: translateY(0); ");
client.println("top: 0; ");
client.println("background-color: #f3f3f3; ");
client.println("transition-delay: 0s; ");
client.println("} ");
client.println(".slider--1 div:last-child { ");
client.println("transform: translateY(0); ");
client.println("bottom: 0; ");
client.println("background-color: #f3f3f3; ");
client.println("border-top: 1px solid #e0e0e0; ");
client.println("transition-delay: 0s; ");
client.println("} ");
client.println(".slider--2 { ");
client.println("background-color: #e6e6e6; ");
client.println("transition-delay: 0.5s; ");
client.println("transform: translateX(0); ");
client.println("border-left: 1px solid #d2d2d2; ");
client.println("} ");
client.println(".slider--3 { ");
client.println("background-color: #d2d2d2; ");
client.println("transition-delay: 1s; ");
client.println("transform: translatex(0); ");
client.println("border-right: 1px solid #d2d2d2; ");
client.println("} ");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<div class='container'>");
client.println("<div class='header'>");
client.println("<h2>LED Control</h2>");
client.println("</div>");
client.println("<div class='content'>");
client.println("<div class='switch-container'>");
client.println("<label class='switch'>");
client.println("<input type='checkbox' class='chbox' onclick='toggleLED()'>");
client.println("<div class='slider slider--0'>ON</div>");
client.println("<div class='slider slider--1'>");
client.println("<div></div>");
client.println("<div></div>");
client.println("</div>");
client.println("<div class='slider slider--2'></div>");
client.println("<div class='slider slider--3'>OFF</div>");
client.println("</label>");
client.println("</div>");
client.println("</div>");
client.println("<div class='footer'>");
client.println("</div>");
client.println("</div>");
client.println("<script>");
client.println("function toggleLED() {");
client.println("var isChecked = document.querySelector('.chbox').checked;");
client.println("var xhttp = new XMLHttpRequest();");
client.println("if (isChecked) {");
client.println("xhttp.open('GET', '/on', true);");
client.println("} else {");
client.println("xhttp.open('GET', '/off', true);");
client.println("}");
client.println("xhttp.send();");
client.println("}");
client.println("function getIPAddress() {");
client.println("var xhttp = new XMLHttpRequest();");
client.println("xhttp.onreadystatechange = function() {");
client.println("if (this.readyState == 4 && this.status == 200) {");
client.println("document.getElementById('ipAddress').innerText = this.responseText;");
client.println("}");
client.println("};");
client.println("xhttp.open('GET', '/ip', true);");
client.println("xhttp.send();");
client.println("}");
client.println("window.onload = function() {");
client.println("getIPAddress();");
client.println("};");
client.println("</script>");
client.println("</body>");
client.println("</html>");

    }

    client.stop();
  }
}
