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
bool connected = false;

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

void ComsSetup(IPAddress serverIP){
  if(ServerMode){
    client = server.available();
    if (client) {
      Serial.println("Client connected.");
      
      String message = client.readStringUntil('\n');  // Read the message sent from the client
      Serial.println("Received message: " + message);
      // Send a response
      client.println("Hello from Server!");
    }
  } else{
    if(!ServerMode && serverIP != IPAddress()) {
      Serial.print("Server found at: ");
      Serial.println(serverIP);
    
      if (client.connect(serverIP, 8080)) {
        Serial.println("Connected to server.");

        // Send a message to the server
        client.println("Hello From Client");

        // Wait for a response
        String response = client.readStringUntil('\n');
        Serial.println("Server response: " + response);
      }
    } else {
    Serial.println("Server not found.");
    }
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
    
    if (client.connect(serverIP, 8080)) {
      Serial.println("Connected to server.");

      // Send a message to the server
      client.println("Hello from Client");

      // Wait for a response
      String response = client.readStringUntil('\n');
      Serial.println("Server response: " + response);
    }
  } else {
    Serial.println("Server not found.");
  }
}

//Variables de Jeux
bool play = false;
bool solo = false;
bool have_ball = false;

int ball[2] = {10, 25};
int ballspeed[2] = {1, 1};
int data[3] = {0, 0, 0}; //Ball[0], ballspeed[0], ballspeed[1]

int paddleX = 56;
bool Leftpressed = false;
bool Rightpressed = false;


// Function to convert integer array to comma-separated string
String data_tostring(int data[3]) {
  return String(data[0]) + "," + String(data[1]) + "," + String(data[2]);
}

// Function to convert a comma-separated string to an integer array
void string_todata(const String& str) {
  int start = 0;  // Start index for each number in the string
  int index = 0;  // Index for each element in the data array

  for (int i = 0; i < str.length(); i++) {
    if (str[i] == ',' || i == str.length() - 1) {  // Check for comma or end of string
      if (i == str.length() - 1) i++;  // Include last character in the substring
      String numStr = str.substring(start, i);
      data[index] = numStr.toInt();  // Convert substring to integer
      index++;
      start = i + 1;  // Move start index to character after comma
    }
  }
}

void Getkeyboard(){
  char input = keypad.getKey();
  Serial.println(input);
  if(keypad.isPressed('B')){
    Leftpressed = true;
    Rightpressed = false;
  } else if(keypad.isPressed('A')){
    Rightpressed = true;
    Leftpressed = false;
  } else if(!keypad.isPressed('B') && !keypad.isPressed('A')){
    Leftpressed = false;
    Rightpressed = false;
  }
}

void DisplayGame(){
  display.clearDisplay();
  //ball
  if(have_ball) {
    display.fillRect(ball[0], ball[1], 2, 2, SSD1306_WHITE);
  }
  //paddle
  display.fillRect(paddleX, SCREEN_HEIGHT - 2, 10, 2, SSD1306_WHITE);
  display.display();
}

void UpdateGame(){
  //player paddle position
  if(Leftpressed){
    paddleX -=3;
  }
  if(Rightpressed){
    paddleX +=3;
  }
  if (paddleX < 0) paddleX = 0;
  if (paddleX > SCREEN_WIDTH - 10) paddleX = SCREEN_WIDTH - 10;

  if(have_ball){
    //ball position
    ball[0] += ballspeed[0];
    ball[1] += ballspeed[1];

    //ball colide border walls
    if(ball[0] <= 0 || ball[0] >= SCREEN_WIDTH){
      ballspeed[0] *= -1;
    }

    //ball colide player paddle
    if (ball[1] >= SCREEN_HEIGHT - 2 && ball[0] >= paddleX && ball[0] <= paddleX + 10) {
      ballspeed[1] *= -1;
    }

    //lost the game
    if(ball[1] >= SCREEN_HEIGHT){
      play = false;
      DisplayMessage("DEFEAT ;(");
      delay(2000);
    }

    //ball to the other player
    if (ball[1] <= 0){
      have_ball = false;
      ballspeed[1] *= -1;
      data[0] = ball[0];
      data[1] = ballspeed[0];
      data[2] = ballspeed[1];
      client.println(data_tostring(data));
    }
  
  } else{
    String response = client.readStringUntil('\n');
    Serial.println("Server response: " + response);
    string_todata(response);
    ball[1] = 0;
    ball[0] = data[0];
    ballspeed[0] = data[1];
    ballspeed[1] = data[2];

    have_ball = true;
  }

}

void setup() {
  //base Setup
  Serial.begin(115200);
  DisplaySetup();
  ModeSelection();
  DisplayMessage(" ");
  WiFiSetup();

  //Card connection
  IPAddress SIP = MDNSSetup();
  ComsSetup(SIP);
}

void loop() {

  //Pong
  play = true;
  DisplayMessage("Waiting for other player");
  while(!connected){
    if(ServerMode){
      client = server.available();
      if(client){
        connected = true;
      }
    } else{
      if(client.connected()){
        connected = true;
      }
    }
  }

  while(connected && play){
    DisplayMessage("Game Starting");
    ball[0] = 10;
    ball[1] = 25;
    paddleX = 56;
    delay(1000);

    if(ServerMode)
    { have_ball = true;} 
    else{ have_ball = false;}
    while(play && connected){
      Getkeyboard();
      UpdateGame();
      DisplayGame();
      delay(45);
    }
  }
}