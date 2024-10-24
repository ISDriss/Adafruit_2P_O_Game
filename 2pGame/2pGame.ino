#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixels.h>
#include <Adafruit_Keypad.h>
#include "credentials.h"

//credentials.h
//#ifndef CREDENTIALS_H
//#define CREDENTIALS_H
//const char* ssid = "WIFI_SSID";   // Replace with your actual SSID
//const char* password = "*PASSWORD"; // Replace with your actual password
//#endif

#define PIN 9
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NEOPIXEL_PIN 9
#define NUM_PIXELS 2
Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void PixelSetup(){
  pixels.begin();
  Wire.begin(5, 6);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); 

  display.clearDisplay();
  display.display();
}

void DisplayMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(msg);
  display.display();
}

// Declaring the pins of the columns and rows of the button matrix
const byte ROWS = 3;
const byte COLS = 3;
const int ROW_PINS[] = {7, 8, 1};  // CHANGE according to connections
const int COL_PINS[] = {20, 10, 0};  // CHANGE according to connections

// Declaring the button actions in a 2D array
const char* keys[ROWS * COLS]] = {
  {"↑",  "→", "A"},   // Row 0: ↑, →, A
  {"←",  "Select", "B"},  // Row 1: ←, Select, B
  {"↓",  nullptr, nullptr}  // Row 2: ↓,
};
Adafruit_Keypad keypad = Adafruit_Keypad(makeKeymap(keys), ROW_PINS, COL_PINS, ROWS, COLS);

WifiServer server(8080);

void WifiSetup() {
  // Serveur
  //Wifi.softAP(ssid,password);
  //server.begin();
  //Serial.println("Serveur démarré");
  
  //Client
  Wifi.begin(ssid,password);
  while(Wifi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connexion au Wifi...");
  }
  Serial.println("Connecté au Wifi");
  client.connect(Wifi.softAPIP(), 8080);
}

void setup() {
  Serial.begin(115200);
  PixelSetup();
  WifiSetup();
}

void loop() {

}