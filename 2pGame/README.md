# Overview DNS
This project sets up a simple "Pong" game on an ESP32 microcontroller using an OLED display, WiFi networking, and a keypad interface. The game can be played between two devices, one acting as a server and the other as a client. The devices communicate via WiFi, and data is exchanged to update the game's state.

## Required Libraries
Make sure you have the following libraries installed in your Arduino IDE:
- `WiFi.h` - For WiFi connectivity.
- `ESPmDNS.h` - For multicast DNS (mDNS) to resolve hostnames to IP addresses.
- `Adafruit_GFX.h` and `Adafruit_SSD1306.h` - To control the OLED display.
- `Keypad.h` - To interface with the button matrix for player input.
- `credentials.h` - Custom header file to store WiFi credentials (`ssid` and `password`).

## Hardware Components
- **ESP32 Development Board**
- **OLED Display (SSD1306)**
- **3x3 Keypad Matrix**
- **WiFi Network**

## Constants and Macros
```cpp
#define PIN 9
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
```
These define the pins and settings for the OLED display. Change these based on your hardware setup.

## Display Functions
- **`DisplaySetup()`**: Initializes the OLED display.
- **`DisplayMessage(String msg)`**: Shows a specific message on the OLED screen.

## Keypad Configuration
- **`byte ROW_PINS[] = {20, 10, 0};`**: Define the GPIO pins connected to the rows of the keypad.
- **`byte COL_PINS[] = {7, 8, 1};`**: Define the GPIO pins connected to the columns of the keypad.
- **Key Mapping**: The keys are mapped as:
  ```cpp
  const char keys[ROWS][COLS] = {
    {'U',  'L', 'D'}, 
    {'R',  'S', '0'},  
    {'A',  'B', '1'}  
  };
  ```
  Modify these pin configurations according to your hardware setup.

## WiFi and Networking Setup
- **`WiFiSetup()`**: Connects to the WiFi network using credentials stored in `credentials.h`.
- **`IPAddress MDNSSetup()`**: Sets up mDNS for identifying devices on the network. The server uses the mDNS name `"esp32server.local"`, and the client resolves this name to find the server.
- **`ComsSetup(IPAddress serverIP)`**: Establishes communication based on whether the device is in server or client mode.

## Game Logic Variables
```cpp
bool play = false;
bool solo = false;
bool have_ball = false;

int ball[2] = {10, 25};
int ballspeed[2] = {1, 1};
int data[3] = {0, 0, 0}; //Ball[0], ballspeed[0], ballspeed[1]

int paddleX = 56;
bool Leftpressed = false;
bool Rightpressed = false;
```
These variables maintain the game state, ball position, speed, and player inputs.

## Mode Selection
**`ModeSelection()`**: Lets the player choose between "HOST" (ServerMode) and "JOIN" (Client Mode) using the keypad. 
- Press **'U'** to host.
- Press **'D'** to join.

## Game Functions
- **`Getkeyboard()`**: Reads inputs from the keypad to control the paddle's movement.
- **`DisplayGame()`**: Renders the game (ball and paddle) on the OLED screen.
- **`UpdateGame()`**: Updates game mechanics like ball movement, paddle collision, wall collision, and sending data to the other player.

## Conversion Functions
- **`data_tostring(int data[3])`**: Converts game data into a string format for transmission.
- **`string_todata(const String& str)`**: Converts received string data into an integer array for game processing.

## Core Setup and Loop
1. **`setup()`**
   - Initializes serial communication, the display, and allows players to select their mode (host/client).
   - Connects to the WiFi network and sets up communication using mDNS.
2. **`loop()`**
   - Continuously checks if a connection is established and manages game states.
   - Handles game mechanics such as paddle control and ball movement.

## Game Logic Breakdown
1. **Paddle Movement**
   - The paddle is controlled via the keypad. The paddle moves left when **'B'** is pressed and right when **'A'** is pressed. 
2. **Ball Mechanics**
   - The ball's position updates based on its current speed and direction.
   - When the ball hits the paddle, it bounces back.
   - If the ball crosses the top of the screen, it is sent to the other player via the client-server communication. 

3. **Communication Between Server and Client**
   - The server accepts client connections and sends game data.
   - The client connects to the server and reads the incoming data to update game states.
   - Data is sent and received in a specific format to ensure synchronized gameplay.

## Game Display Example
1. **Defeat Message**: If the player fails to hit the ball back, a "DEFEAT ;(" message is shown.
2. **Game Start**: Both players see a "Game Starting" message when a connection is established.

## Notes on Usage
- Ensure both devices are connected to the same WiFi network.
- Adjust the pin configurations based on your actual wiring setup.
- Make sure the `credentials.h` file is correctly set up with your WiFi details.

## Improvements and Adjustments
- **Error Handling**: Implement more robust error checks for WiFi connectivity and mDNS resolution.
- **Game Expansion**: Add more features, like scoring, to make the game more interesting.
- **Keypad Control Enhancement**: Consider using debouncing to handle multiple key presses effectively.

This documentation should help you understand how each part of the project functions, facilitating smooth setup and operation. Let me know if any additional sections need further clarification or if you want to expand the project!
