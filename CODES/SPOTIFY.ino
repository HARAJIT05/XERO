#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "spotify.h"  // Include the header file with the Spotify logo data

char *WIFI_SSID = "HARAJIT";
char *WIFI_PASS = "12345678";

String client_id = "54808ab6e83e44edaf8aa87af9bd66dd";
String redirect = "http%3A%2F%2Fhttpbin.org%2Fanything";
String cookie = "__Host-device_id=AQBXg2O0DiN2cLxUGxMOHlGyNhOuD5WUv5lGe2NFPlPTobQ0pgnVKbuzj_MNZ9jgKEyCaEQv6EWJ9VkYpnmwu7fAmo7zOYG09BA; inapptestgroup=; sp_tr=false; __Secure-TPASESSION=AQC/nC5T+kLFw4VaQq/dJPSRA8z2s3GqmM4d2ulgw6mRbeVKdZbaiUKMCgGl2bKVJvOkL9dXbc9FRe5tb8JdH/Yz54gaVVGjxL8=; remember=harajit05%40gmail.com; sp_dc=AQAPa-JeEFhXBnV4L5RBoQC9YCTwRrC7-4GYdci2tgfd3lyolRVlowjJJhBhR4ve-JPvg6b2kxcNQ19llLGRVr8ue7D_vs2fvsnyuPAmdvNHq4Yw-atikqZe5rIhEHD-WtP74oYyB9lfjiymUbzJnqL3t9md-p0; sp_sso_csrf_token=013acda71983d72c3fcc54937dc1d574e48699273b31373036393835373835393238; sp_key=54d3ccc7-0056-4ac5-8535-0d2e0886e113; __Host-sp_csrf_sid=577a8be2850ab2f07ca0d6991d5018ed23b6b828e82900fcb67f08e85bbd49c1; csrf_token=AQCp3IE36GX1MZMvSetF4YKgie9nn4kBYc-lKki3ovL5cSBzapKGkepNnDhTIuy2ALh8NaYISlmP0P_P9uP2gi_pUlWqMqC-nOrWjc2wLu2T8SiBhg";

long current = 0, duration = 0;
int volume;
String artists = "", title, album, device, type;
bool isActive, playing;

bool isExpired = false;
String token = "BQBDbvlke7xRNXwPFlcUKH6QJLjjuKXPYn4tlyVGvuAAyTway24n62VmWVEnAAaspFDUwHl2q6vNFPp0Q0XVmfiTPqJNKq3dBfu5S8JZhOUXyby-0b8PotXDjTndFzx2MSQr8wS5pU8im1xPpVTt1uldBJqGQbRA6FHW4GpRYomyYt8-I6k8pUM7WZpGQpvBTH7sHZ5R7h4";

WiFiMulti wifiMulti;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3c // Replace with your OLED I2C address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_ADDRESS);

extern uint8_t SmallFont[], MediumNumbers[];

bool shouldUpdateProgressBar = true;

void displaySpotifyLogo() {
  display.clearDisplay();
  display.drawBitmap(0, 0, spotify, 128, 64, 1);  // Assuming zz is the array in spotify.h and '1' for monochrome
  display.display();
  delay(3000);  // Adjust the delay as needed
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting device");
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  displaySpotifyLogo();  // Display Spotify logo on startup

  Serial.print("Waiting for WiFi to connect...");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 12);
  display.print(F("Connecting to WiFi..."));
  display.display();

  while ((wifiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 12);
  display.print(F(""));
  display.display();

  getToken();
}

void loop() {
  if (isExpired) {
    getToken();
  }

  getPlayer();

  if (playing) {
    updateScreen();
  } else {
    // Stop updating progress bar when the track ends
    shouldUpdateProgressBar = false;
  }
}

void updateScreen() {
  if (!shouldUpdateProgressBar) {
    return;  // Skip updating the progress bar if not necessary
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(title);
  display.setCursor(0, 12);
  display.print(artists);
  display.setCursor(0, 24);
  display.print(album);
  display.setCursor(0, 36);
  display.print(device + " " + String(volume));

  // Draw background for progress bar
  display.fillRect(0, 48, 127, 52, SSD1306_BLACK);

  // Draw filled portion of the progress bar
  int pr = map(current, 0, duration, 0, 127);
  display.fillRect(0, 48, pr, 52, SSD1306_WHITE);

  // Draw border for progress bar
  display.drawRect(0, 48, 127, 52, SSD1306_WHITE);

  display.setCursor(0, 56);
  display.print(playing ? ">" : "||");

  int m = duration / 60000;
  int s = (duration % 60000) / 1000;
  display.setCursor(64, 56);
  display.print(String(m) + ":" + (s < 10 ? "0" : "") + String(s));

  display.display();
}

void getToken() {
  HTTPClient http;
  String url = "https://accounts.spotify.com/authorize?response_type=token&redirect_uri=" + redirect + "&client_id=" + client_id + "&scope=user-read-playback-state+user-read-playback-position+user-modify-playback-state&state=cryq3";
  Serial.println(url);
  http.begin(url);
  http.addHeader("Cookie", cookie);
  int httpCode = http.GET();
  String payload = http.getLocation();
  token = payload.substring(payload.indexOf("access_token=") + 13, payload.indexOf("&token_type"));
  Serial.print("Token: ");
  Serial.println(token);
  http.end();
}

void getPlayer() {
  HTTPClient http;
  http.begin("https://api.spotify.com/v1/me/player");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", "Bearer " + token);
  int httpCode = http.GET();
  String payload = http.getString();

  if (httpCode == HTTP_CODE_OK) {
    DynamicJsonDocument json(20000);
    deserializeJson(json, payload);

    title = json["item"]["name"].as<String>();
    album = json["item"]["album"]["name"].as<String>();
    artists = "";
    JsonArray arr = json["item"]["artists"].as<JsonArray>();
    for (JsonVariant value : arr) {
      artists += value["name"].as<String>() + " ";
    }

    current = json["progress_ms"].as<long>();
    duration = json["item"]["duration_ms"].as<long>();
    playing = json["is_playing"].as<bool>();

    device = json["device"]["name"].as<String>();
    isActive = json["device"]["is_active"].as<bool>();
    type = json["device"]["type"].as<String>();
    volume = json["device"]["volume_percent"].as<int>();

    isExpired = false;
    
  } else {
    current = 0, duration = 10;
    volume = 0;
    artists = "", title = "ERROR", album = "Token expired", device = "", type = "";
    isActive = false, playing = false;
    isExpired = true;
  }

  http.end();
}