#include <WiFi.h>
#include <ESPmDNS.h>
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
WiFiClient client;
bool ServerMode = false;

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

IPAddress MDNSSetup() {
  if(ServerMode){
    // Start mDNS
    if (!MDNS.begin("esp32server")) {  // Register the device as "esp32server.local"
      Serial.println("Error setting up MDNS responder!");
    } else {
      Serial.println("mDNS responder started");
    }
    // Start server
    server.begin();
    return IPAddress();

  } else{
    // Use mDNS to find the server
    if (!MDNS.begin("esp32client")) {
      Serial.println("Error starting mDNS on client");
    }
    // Resolve server's mDNS hostname to IP
    Serial.println("Resolving server...");
    IPAddress serverIP = MDNS.queryHost("esp32server");
    return serverIP;
  }
}

void ServerLoop() {
  client = server.available();  // Check if a client has connected
  
  if (client) {
    Serial.println("Client connected.");
    String message = client.readStringUntil('\n');  // Read the message sent from the client
    Serial.println("Received message: " + message);

    // Send a response
    client.println("Hello from Server!");
    
    client.stop();  // Close the connection
  }
}

void ClientSetup(IPAddress serverIP) {
  if (serverIP) {
    Serial.print("Server found at: ");
    Serial.println(serverIP);
    
    WiFiClient client;
    if (client.connect(serverIP, 80)) {
      Serial.println("Connected to server.");

      // Send a message to the server
      client.println("Hello from Client!");

      // Wait for a response
      String response = client.readStringUntil('\n');
      Serial.println("Server response: " + response);
      
      client.stop();  // Close the connection
    }
  } else {
    Serial.println("Server not found.");
  }
}

void setup() {
  Serial.begin(115200);
  DisplaySetup();
  ModeSelection();
  DisplayMessage(" ");
  WiFiSetup();


  IPAddress SIP = MDNSSetup();
  if(!ServerMode and SIP != IPAddress()){
    ClientSetup(SIP);
  }
}

void loop() {
  if(ServerMode){
    ServerLoop();
  }
}