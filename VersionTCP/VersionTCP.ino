#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuration Wi-Fi
const char *ssid = "PoleDeVinci_IFT";
const char *password = "*c.r4UV@VfPn_0";
WiFiClient client;
WiFiServer server(8080);

IPAddress serverIP(10, 1, 224, 86);        // Adresse IP de l'ESP32 Serveur
IPAddress allowedClientIP(10, 1, 224, 36); // IP spécifique autorisée

// Configuration du clavier matriciel avec Adafruit Keypad
const byte ROWS = 3; // Trois lignes
const byte COLS = 3; // Trois colonnes
byte COL_PINS[ROWS] = {7, 8, 1};
byte ROW_PINS[COLS] = {20, 10, 0};
char keys[ROWS][COLS] = {
    {'U', 'L', 'D'},
    {'R', 'S', 'R'},
    {'A', 'B', '9'}};

Adafruit_Keypad keypad = Adafruit_Keypad(makeKeymap(keys), ROW_PINS, COL_PINS, ROWS, COLS);

// Variables de jeu
int paddleX = 56;
int opponentPaddleX = 56;
int ballX = 64, ballY = 32;
int ballSpeedX = 1, ballSpeedY = 1;
bool gameRunning = true;
bool isServer = false;
bool clientConnected = false;
bool playAlone = false;
bool leftButtonPressed = false;
bool rightButtonPressed = false;

void setup()
{
  // Initialisation de l'écran
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();
  // Initialisation du clavier matriciel
  keypad.begin();

  // Initialisation Wi-Fi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connexion au WiFi...");
  }
  Serial.println("Connecté au WiFi");
  delay(500); // Pause pour permettre la stabilisation

  // Demander à l'utilisateur de choisir s'il souhaite être serveur ou client à l'aide des boutons du keypad
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Appuyez sur 'A' pour Serveur\n'B' pour Client");
  display.display();

  while (true)
  {
    keypad.tick();
    while (keypad.available())
    {
      keypadEvent e = keypad.read();
      if (e.bit.EVENT == KEY_JUST_PRESSED)
      {
        if (e.bit.KEY == 'A')
        { // Choix du Serveur
          isServer = true;
          server.begin();
          Serial.println("Mode Serveur");
          Serial.println("Adresse IP: " + WiFi.localIP().toString());
          display.clearDisplay();
          display.print("Mode Serveur\nIP: ");
          display.print(WiFi.localIP());
          display.display();
          delay(2000);
          display.clearDisplay();
          display.setTextSize(1);
          display.setCursor(0, 0);
          display.print("En attente de client...\nAppuyez sur 'A' pour jouer seul");
          display.display();

          while (true)
          {
            keypad.tick();
            while (keypad.available())
            {
              keypadEvent e = keypad.read();
              if (e.bit.EVENT == KEY_JUST_PRESSED && e.bit.KEY == 'A')
              {
                playAlone = true;
                break;
              }
            }
            if (playAlone)
            {
              break;
            }
            WiFiClient remoteClient = server.available();
            if (remoteClient && remoteClient.connected())
            {
              if (remoteClient.remoteIP() == allowedClientIP)
              {
                clientConnected = true;
                client = remoteClient; // Associer le client
                break;
              }
            }
          }
          break;
        }
        else if (e.bit.KEY == 'B')
        { // Choix du Client
          isServer = false;
          Serial.println("Mode Client");
          Serial.println("Adresse IP: " + WiFi.localIP().toString());
          display.clearDisplay();
          display.print("Mode Client\nIP: ");
          display.print(WiFi.localIP());
          display.display();
          delay(2000);
          // Connexion au serveur
          // while (!client.connect(WiFi.gatewayIP(), 8080)) {
          while (!client.connect(serverIP, 8080))
          {
            Serial.println("Tentative de connexion au Serveur...");
            delay(1000);
          }
          Serial.println("Connecté au Serveur");
        }
      }
    }
    if (isServer || client.connected())
    {
      Serial.println("Lancement du jeu");

      break;
    }
  }
}

void loop()
{
  if (!gameRunning)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 25);
    display.print(isServer ? "Victoire!" : "Defaite!");
    display.display();

    keypad.tick();
    while (keypad.available())
    {
      keypadEvent e = keypad.read();
      if (e.bit.EVENT == KEY_JUST_PRESSED)
      {
        gameRunning = true;
        ballX = 64;
        ballY = 32;
        ballSpeedX = 1;
        ballSpeedY = 1;
        return;
      }
    }
    delay(10);
    return;
  }

  // Gestion des commandes du clavier
  keypad.tick();
  while (keypad.available())
  {
    keypadEvent e = keypad.read();
    if (e.bit.EVENT == KEY_JUST_PRESSED)
    {
      if (e.bit.KEY == 'B')
      { // Bouton gauche pressé
        leftButtonPressed = true;
      }
      else if (e.bit.KEY == 'A')
      { // Bouton droit pressé
        rightButtonPressed = true;
      }
    }
    else if (e.bit.EVENT == KEY_JUST_RELEASED)
    {
      if (e.bit.KEY == 'B')
      { // Bouton gauche relâché
        leftButtonPressed = false;
      }
      else if (e.bit.KEY == 'A')
      { // Bouton droit relâché
        rightButtonPressed = false;
      }
    }
  }

  if (leftButtonPressed)
  {
    paddleX -= 4;
  }
  if (rightButtonPressed)
  {
    paddleX += 4;
  }
  paddleX = constrain(paddleX, 0, SCREEN_WIDTH - 10);
  if (isServer)
  {
    // Serveur: mise à jour de la position de la balle
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Collision avec les bords gauche et droit
    if (ballX <= 0 || ballX >= SCREEN_WIDTH)
    {
      ballSpeedX *= -1;
    }

    // Collision avec la palette du joueur
    if (ballY >= SCREEN_HEIGHT - 2 && ballX >= paddleX && ballX <= paddleX + 10)
    {
      ballSpeedY *= -1;
      ballY = SCREEN_HEIGHT - 3;
    }

    // Collision avec la palette de l'adversaire
    if (ballY <= 2 && ballX >= opponentPaddleX && ballX <= opponentPaddleX + 10)
    {
      ballSpeedY *= -1;
      ballY = 3;
    }

    // Vérifier si la balle sort de l'écran (victoire/défaite)
    if (ballY >= SCREEN_HEIGHT)
    {
      gameRunning = false;
    }
    if (ballY <= 0)
    {
      gameRunning = false;
    }

    // Envoyer la position de la balle et de la palette aux clients
    if (client.connected())
    {
      client.print(String(ballX) + "," + String(ballY) + "," + String(paddleX) + "\n");
    }
  }
  else
  {
    // Client: recevoir la position de la balle et de la palette du serveur
    if (client.connected() && client.available())
    {
      String data = client.readStringUntil('\n');
      int firstComma = data.indexOf(',');
      int secondComma = data.indexOf(',', firstComma + 1);

      if (firstComma != -1 && secondComma != -1)
      {
        ballX = SCREEN_WIDTH - data.substring(0, firstComma).toInt();                // Inverser la position X
        ballY = SCREEN_HEIGHT - data.substring(firstComma + 1, secondComma).toInt(); // Inverser la position Y
        opponentPaddleX = SCREEN_WIDTH - data.substring(secondComma + 1).toInt();
      }
    }
  }

  // Envoyer la position de la palette au serveur
  if (client.connected())
  {
    client.print(String(paddleX) + "\n");
  }

  // Affichage
  display.clearDisplay();
  display.fillRect(paddleX, SCREEN_HEIGHT - 2, 10, 2, SSD1306_WHITE); // Palette du joueur en bas de l'écran
  display.fillRect(opponentPaddleX, 0, 10, 2, SSD1306_WHITE);         // Palette de l'adversaire en haut de l'écran
  display.fillRect(ballX, ballY, 2, 2, SSD1306_WHITE);                // Balle
  display.display();

  delay(5);
}