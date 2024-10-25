#include <WiFi.h>
//#include <ESPmDNS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include "credentials.h"
  //credentials.h
  //#ifndef CREDENTIALS_H
  //#define CREDENTIALS_H
  //const char* ssid = "WIFI_SSID";   // Replace with your actual SSID
  //const char* password = "*PASSWORD"; // Replace with your actual password
  //#endif
//

#define PIN 9
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void DisplaySetup(){
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
byte ROW_PINS[] = {20, 10, 0};  // CHANGE according to connections
byte COL_PINS[] = {7, 8, 1};    // CHANGE according to connections

// Declaring the button actions in a 2D array
const char keys[ROWS][COLS] = {
  {'U',  'L', 'D'},   // Row 0: ↑, →, A
  {'R',  'S', '0'},  // Row 1: ←, Select, B
  {'A',  'B', '1'}  // Row 2: ↓,
};
Keypad keypad = Keypad(makeKeymap(keys), ROW_PINS, COL_PINS, ROWS, COLS);

WiFiServer server(8080);
bool ServerMode = false;

void WiFiSetup() {
  WiFi.begin(ssid,password);
  Serial.print("Connecting to Wifi");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wifi");
  Serial.print("At IP:");
  Serial.println(WiFi.localIP());
}

// Function to handle mode selection
void ModeSelection() {
  DisplayMessage("HOST: Up\nJOIN: Down");
  
  while (true) {  // Keep looping until a mode is selected
    char input = keypad.getKey();
    if (input){
      Serial.println(input);
      if(input == 'U'){
        ServerMode = true;
        DisplayMessage("HOST");
        delay(500);
        return;
      } else if(input == 'D'){
        ServerMode = false;
        DisplayMessage("JOIN");
        delay(500);
        return;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  DisplaySetup();

  ModeSelection();
  DisplayMessage(" ");

  WiFiSetup();
}

void loop() {

}