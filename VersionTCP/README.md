# Overview TCP
This project is a networked game using an ESP32 microcontroller. It allows two players to play a Pong-like game using an OLED display and a keypad for controls. The game can be set up for two modes: a server mode (where the ESP32 acts as the host and waits for a client to connect) or a client mode (where the ESP32 connects to an existing server). The game operates via Wi-Fi and allows for gameplay between two devices over a network. The player controls the paddle using the A and B buttons on a 3x3 keypad, and the game logic ensures smooth gameplay and synchronization between the server and client.

## Components Required
ESP32 microcontroller  
0.96" I2C OLED display (128x64 pixels)  
Adafruit Keypad (3x3 matrix)  
Wi-Fi network  
Power source  

## Libraries Used
WiFi.h - To handle the Wi-Fi connection.  
Adafruit_GFX.h - Core graphics library for the OLED display.  
Adafruit_SSD1306.h - Driver for the OLED display.  
Adafruit_Keypad.h - Library for managing the 3x3 keypad input.  

## Pin Configuration
OLED Display: Connected via I2C (SDA, SCL)  
Keypad:  
Rows: Pins 7, 8, 1  
Columns: Pins 20, 10, 0  

## Key Features
Wi-Fi Configuration: Connects to a specified Wi-Fi network using predefined SSID and password. The user can choose whether the ESP32 will act as a server or a client.  
OLED Display: Provides visual feedback to players, showing connection status, mode selection, and game status.  
Keypad Control: Allows players to control the paddle movement and choose game modes.  
Network Gameplay:  
Server Mode: Initializes a server and waits for a client connection. Updates game state and sends data to connected clients.  
Client Mode: Connects to the server's IP address and synchronizes game state based on data received.  

## Game Variables
paddleX and opponentPaddleX: X-coordinates of the player’s paddle and opponent’s paddle.  
ballX and ballY: Coordinates of the ball on the screen.  
ballSpeedX and ballSpeedY: Speed of the ball in X and Y directions.  
isServer: Boolean flag to indicate whether the ESP32 is acting as a server.  
clientConnected: Boolean flag to indicate if a client is connected to the server.  
playAlone: Boolean flag to determine if the player is playing against AI (local mode).  *

## Functions
### setup()
Initialize OLED Display: Sets up the screen for displaying information.  
Initialize Keypad: Begins the keypad to detect user input.  
Wi-Fi Connection: Connects to the specified Wi-Fi network.  
Mode Selection: Displays instructions on the OLED for choosing between Server ('A') or Client ('B') mode.  
If the 'A' key is pressed, the device will act as a server, set up a Wi-Fi server, and wait for a client to connect.  
If the 'B' key is pressed, the device will attempt to connect to a server using a pre-defined IP address.  

### loop()
### Game Logic Execution:
Continuously checks for input from the keypad to move the paddle.  
Updates the ball's position based on its speed and direction.  
Manages collisions with the paddles and screen borders.  
Server Mode: Sends updated game state to the client.  
Client Mode: Receives updated game state from the server and updates the display.  

### Keypad Event Handling:
Moves paddles left or right based on key inputs.  

### Display Update:
Shows the paddles and ball on the OLED screen in real-time.  

## Code Walkthrough   
### Wi-Fi Configuration   
The ESP32 connects to the network using:   

WiFi.begin(ssid, password);  
while (WiFi.status() != WL_CONNECTED) {  
    delay(1000);  
    Serial.println("Connexion au WiFi...");  
}  

Note: The SSID and password are hard-coded and should be changed as per your Wi-Fi network.  

### Game Mode Selection
The game prompts the user on the OLED screen to select between server or client mode:

display.print("Appuyez sur 'A' pour Serveur\n'B' pour Client");  

If the 'A' button is pressed, the ESP32 becomes a server; if 'B' is pressed, it becomes a client and attempts to connect to a server using the serverIP address.  

### Game Controls
The keypad allows the following controls:  

'A' Button: Move paddle right  
'B' Button: Move paddle left  

### Paddle and Ball Movement Logic
The ball's movement is updated by:  

ballX += ballSpeedX;  
ballY += ballSpeedY;  
The game checks for collisions and adjusts the ball's speed direction (ballSpeedX, ballSpeedY) accordingly.  

### Display Logic
Real-time updates are sent to the OLED screen to display the paddles and ball:  

display.clearDisplay();  
display.fillRect(paddleX, SCREEN_HEIGHT - 2, 10, 2, SSD1306_WHITE);  
display.fillRect(opponentPaddleX, 0, 10, 2, SSD1306_WHITE);  
display.fillRect(ballX, ballY, 2, 2, SSD1306_WHITE);  
display.display();  

## Error Handling
The project has basic error handling, such as:  

Failed OLED Initialization: If the OLED fails to initialize, the setup enters an infinite loop.  
Wi-Fi Connection Issues: The ESP32 retries connecting to Wi-Fi until successful.  

## Potential Improvements
Dynamic IP Input: Modify the code to allow input of the server IP address via the keypad.  
Score Display: Implement a scoring system to display on the OLED.  
Game Modes: Extend to different game modes (e.g., solo against AI, multiplayer with more than two players).  
Wi-Fi Security: Secure Wi-Fi credentials handling (currently hardcoded).  

## Conclusion
This project leverages the ESP32's networking capabilities along with an OLED display and keypad for a fun, interactive game. It illustrates real-time data communication between two devices using Wi-Fi, making it an excellent demonstration of networking with microcontrollers.  