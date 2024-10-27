
# Overview UDP
This project implements a simple Pong game on an ESP32, using an OLED display to show the game, a keypad to control the paddles, and WiFi UDP communication to connect two devices, enabling a multiplayer mode. One ESP32 acts as a server, and the other as a client. The server manages the game state, including the ball's movement, and synchronizes this state with the client.

## Components
1. **ESP32**
2. **OLED Display (Adafruit SSD1306, 128x64)**
3. **Adafruit Keypad**
4. **WiFi (UDP Communication)**

## Libraries
- `WiFi.h`: Manages the WiFi connection for the ESP32.
- `WiFiUdp.h`: Enables UDP communication between server and client.
- `Adafruit_GFX.h` & `Adafruit_SSD1306.h`: Controls the OLED display.
- `Adafruit_Keypad.h`: Interfaces with a 3x3 matrix keypad.

## Wiring and Configuration

1. **OLED Display:**
   - Connect the OLED via I2C (SDA to GPIO21, SCL to GPIO22).

2. **Keypad:**
   - **Rows**: Connect pins to GPIO20, GPIO10, and GPIO0.
   - **Columns**: Connect pins to GPIO7, GPIO8, and GPIO1.

3. **WiFi Setup:**
   - Define SSID and password for the WiFi network.
   - Assign fixed IPs to both server and client ESP32s.

4. **UDP Communication Ports:**
   - Server port: `8080`
   - Client port: `8081`

## Game Mechanics
- The game is played between two players, one acting as the server and the other as the client.
- Players use keys on the keypad to move their paddles left ('B') and right ('A').
- The server handles the game logic, updating the ball's position, checking for collisions, and determining when the game ends.
- The server and client communicate via UDP, sending paddle positions and ball coordinates to each other.

## Code Explanation

1. **Initialization (`setup()`):**
   - Start Serial communication for debugging.
   - Initialize OLED and Keypad.
   - Connect to WiFi.
   - Use the keypad to decide if the device will function as a server or client.
   - If acting as a server, it waits for the client to connect via UDP. If acting as a client, it sends a "Hello Server" message to initiate communication.

2. **Game Loop (`loop()`):**
   - **Keypad Control:** Poll the keypad to check if left or right buttons are pressed, and adjust paddle position accordingly.
   - **Game Logic (Server):** 
     - Move the ball based on speed variables, check for collisions, and reverse the ball's direction if needed.
     - Handle collisions with the player's paddle and opponent's paddle.
     - Send updated game data to the client.
   - **Game Logic (Client):**
     - Receive game state from the server, including ball and opponent paddle positions.
     - Update display accordingly.
   - **Display Rendering:** Update the OLED screen to show paddle positions and the ball.

3. **Game Over Conditions:**
   - When the ball goes off the screen (bottom or top), the game is considered over, and the winner is declared.
   - Players can restart the game by pressing a keypad button.

## Detailed Code Description

1. **Global Variables and Configuration:**
   - Variables for WiFi configuration, UDP communication, and game controls.
   - Game variables track the paddle positions, ball position, and speed.
   - State variables determine if the device acts as a server or client and track whether the game is running.

2. **Key Functions:**
   - `keypad.tick()`: Scans for key presses and manages paddle controls.
   - `udp.beginPacket()`, `udp.write()`, `udp.endPacket()`: Manage sending and receiving data between server and client.

3. **OLED Display Management:**
   - The `Adafruit_SSD1306` library is used to draw the paddles and ball on the screen.
   - `display.clearDisplay()` and `display.display()` are used to refresh the screen.

## Setup and Run Instructions

1. **Hardware Assembly:**
   - Connect the OLED display, keypad, and ESP32 as per the wiring details.
   - Ensure the ESP32 has access to a WiFi network.

2. **Software Setup:**
   - Install required Arduino libraries: `Adafruit GFX`, `Adafruit SSD1306`, `Adafruit Keypad`, `WiFi`, `WiFiUdp`.
   - Flash the code to two ESP32 devices. Assign one as a server and the other as a client.

3. **Game Start:**
   - Power up both ESP32s.
   - The devices will connect to the WiFi and display a menu to choose between server ('A') or client ('B').
   - Once selected, the server waits for the client to connect. 
   - Upon connection, the game begins, and the devices communicate to sync game states.

## Troubleshooting Tips

- **WiFi Connection Issues:** Ensure the SSID and password are correct, and both ESP32s are on the same network.
- **OLED Display Not Working:** Check I2C connections and ensure the correct I2C address (0x3C) is set.
- **Keypad Input Not Detected:** Verify GPIO pin connections and keypad initialization.
- **UDP Communication Failing:** Make sure IP addresses and ports are correctly assigned on both devices.

## Future Enhancements

1. **Improved Collision Detection:**
   - Enhance the collision algorithms for better accuracy and gameplay smoothness.

2. **Game Mode Expansion:**
   - Add different game modes or challenges (e.g., increased ball speed over time).

3. **UI Enhancements:**
   - Improve the user interface with more graphics, scores, and game status on the OLED.

## Conclusion
This project showcases an integration of multiple peripherals and network communication to create a simple, yet engaging game. It can be expanded to include additional features, and serves as a great example of using ESP32's versatile capabilities.
