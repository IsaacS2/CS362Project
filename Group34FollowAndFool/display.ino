// Isaac Sanchez and Sebastian Ho
//
// Library and some defined header values written by prenticedavid on Github: https://github.com/prenticedavid
// The header values and comments were from example code from both the graphicstest.ino file in the example files for the
// Adafruit TFTLCD Library and the graphicstest_kbv.ino file in the example files for the MCUFRIEND_kbv Library.
// Luzso is also listed as a library contributor on Github: https://github.com/Luzso
// The MCUFRIEND_kbv Library inherits most of its data from the Adafruit_GFX Library which we also downloaded for this
// project from the ArduinoIDE.
// The SPI Library is also used in this code.
// MCUFRIEND_kbv Library link: https://github.com/prenticedavid/MCUFRIEND_kbv
//
// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
//

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternatively connect to Arduino's reset pin

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include <string.h>
MCUFRIEND_kbv tft;

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define FAKECOLOR 0xC638
#define GRAY    0x595F69
#define BLUEMAIN 0x65A4FC

//
// Player State Values
//
#define IDLE 1
#define RIGHT 2
#define LEFT 3
#define UP 4
#define DOWN 5
#define BUTTON 6
#define FAKE 7
#define LOST 8
#define WIN 9
#define CORRECT 10 // correct input for the specific round
#define LIVELOST 11
#define DRAW 12
#define NONE 13
#define FAKEUP 14
#define FAKELEFT 15
#define FAKEDOWN 16
#define FAKERIGHT 17
#define FAKEBUTTON 18



// Screen
int screenWidth;
int screenHeight;

// Input
int incomingByte = 0;

// Buzzer
const int buzzerPin = 10;

// Player 1
int playerOneState = IDLE;
int playerOneResult = NONE;
uint16_t playerOneColor = WHITE;
int playerOneX;
int playerOneY;
int playerOneLives;
int playerOneFake = 1;
int playerOneReady = 0;

// Player 2
int playerTwoState = IDLE;
int playerTwoResult = NONE;
uint16_t playerTwoColor = WHITE;
int playerTwoX;
int playerTwoY;
int playerTwoLives;
int playerTwoFake = 1;
int playerTwoReady = 0;

// CPU
int cpuState = IDLE;
uint16_t cpuColor = WHITE;
int cpuX;
int cpuY;

// Sizes
int arrowSize = 15;
int cpuArrowSize = 25;
int lifeSize = 10;

// Timer
unsigned long fakeStart = 0; // will store last time the timer started
unsigned long timer = 3000;
unsigned long maxCPUDelay = 3000;  // maximum time before CPU issues a command
unsigned long minCPUDelay = 2000;  // minimum time before CPU issues a command
unsigned long cpuDelay;
unsigned long roundTime;
unsigned long downTime = 10;
unsigned long fakeDelay = 2000;
unsigned long roundStart;
unsigned long resultScreenDelay = 5000;
unsigned long startScreenDelay = 2000;
unsigned long gameStart;


//
// drawPlayerOne
//
// This function draws the first player's (player to the left of the screen) current status on the TFT screen.
//
void drawPlayerOne()
{
  tft.fillCircle(playerOneX, playerOneY, 40, playerOneColor);

  if(playerOneState == UP)
  {
    tft.fillTriangle(playerOneX - arrowSize, playerOneY + arrowSize, playerOneX + arrowSize, playerOneY + arrowSize, playerOneX, playerOneY - arrowSize, BLACK);     
  }
  if(playerOneState == LEFT)
  {
    tft.fillTriangle(playerOneX + arrowSize, playerOneY - arrowSize, playerOneX + arrowSize, playerOneY + arrowSize, playerOneX - arrowSize, playerOneY, BLACK);     
  }
  if(playerOneState == DOWN)
  {
    tft.fillTriangle(playerOneX - arrowSize, playerOneY - arrowSize, playerOneX + arrowSize, playerOneY - arrowSize, playerOneX, playerOneY + arrowSize, BLACK);     
  }  
  if(playerOneState == RIGHT)
  {
    tft.fillTriangle(playerOneX - arrowSize, playerOneY - arrowSize, playerOneX - arrowSize, playerOneY + arrowSize, playerOneX + arrowSize, playerOneY, BLACK);     
  } 
  if(playerOneState == BUTTON)
  {
    tft.fillCircle(playerOneX, playerOneY, 30, BLACK);
  } 
}


//
// drawPlayerTwo
//
// This function draws the second player's (player to the right of the screen) current status on the TFT screen.
//
void drawPlayerTwo()
{
  tft.fillCircle(playerTwoX, playerTwoY, 40, playerTwoColor);

  if(playerTwoState == UP)
  {
    tft.fillTriangle(playerTwoX - arrowSize, playerTwoY + arrowSize, playerTwoX + arrowSize, playerTwoY + arrowSize, playerTwoX, playerTwoY - arrowSize, BLACK);     
  }
  if(playerTwoState == LEFT)
  {
    tft.fillTriangle(playerTwoX + arrowSize, playerTwoY - arrowSize, playerTwoX + arrowSize, playerTwoY + arrowSize, playerTwoX - arrowSize, playerTwoY, BLACK);     
  }
  if(playerTwoState == DOWN)
  {
    tft.fillTriangle(playerTwoX - arrowSize, playerTwoY - arrowSize, playerTwoX + arrowSize, playerTwoY - arrowSize, playerTwoX, playerTwoY + arrowSize, BLACK);     
  }  
  if(playerTwoState == RIGHT)
  {
    tft.fillTriangle(playerTwoX - arrowSize, playerTwoY - arrowSize, playerTwoX - arrowSize, playerTwoY + arrowSize, playerTwoX + arrowSize, playerTwoY, BLACK);     
  }
  if(playerTwoState == BUTTON)
  {
    tft.fillCircle(playerTwoX, playerTwoY, 30, BLACK);
  }
}


//
// drawCPU
//
// This function draws the CPU's (character in the middle of the screen, AKA the leader) current status on the TFT screen.
//
void drawCPU()
{
  tft.fillCircle(cpuX, cpuY, 50, cpuColor);

  if(cpuState == UP || cpuState == FAKEUP)
  {
    tft.fillTriangle(cpuX - cpuArrowSize, cpuY + cpuArrowSize, cpuX + cpuArrowSize, cpuY + cpuArrowSize, cpuX, cpuY - cpuArrowSize, ~cpuColor);     
  }
  if(cpuState == LEFT || cpuState == FAKELEFT)
  {
    tft.fillTriangle(cpuX + cpuArrowSize, cpuY - cpuArrowSize, cpuX + cpuArrowSize, cpuY + cpuArrowSize, cpuX - cpuArrowSize, cpuY, ~cpuColor);     
  }
  if(cpuState == DOWN || cpuState == FAKEDOWN)
  {
    tft.fillTriangle(cpuX - cpuArrowSize, cpuY - cpuArrowSize, cpuX + cpuArrowSize, cpuY - cpuArrowSize, cpuX, cpuY + cpuArrowSize, ~cpuColor);     
  }  
  if(cpuState == RIGHT || cpuState == FAKERIGHT)
  {
    tft.fillTriangle(cpuX - cpuArrowSize, cpuY - cpuArrowSize, cpuX - cpuArrowSize, cpuY + cpuArrowSize, cpuX + cpuArrowSize, cpuY, ~cpuColor);     
  }
  if(cpuState == BUTTON || cpuState == FAKEBUTTON)
  {
    tft.fillCircle(cpuX, cpuY, 40, ~cpuColor);
  }
}


//
// drawLives
//
// This function draws the current amount of lives for both players on the TFT screen.
//
void drawLives()
{
  for(int i = 0; i < playerOneLives; i++)
  {
    tft.fillCircle(lifeSize + (i * lifeSize * 3), screenHeight - lifeSize - 1, lifeSize, RED);
  }

  for(int i = 0; i < playerTwoLives; i++)
  {
    tft.fillCircle(screenWidth - (lifeSize + (i * lifeSize * 3)), screenHeight - lifeSize - 1, lifeSize, RED);
  }
}


//
// startScreen
//
// This function draws the start screen for the game, called "Follow And Fool".
//
void startScreen() {
  tft.setCursor((screenWidth/4) - 6, (screenHeight/2));
  tft.setTextColor(GRAY);
  tft.setTextSize(3);
  tft.print("Follow And Fool!");
}


//
// readyUp
//
// This function is for the logic behind the ready screen.
// Both players must be ready for the game to continue.
//
void readyUp() {
  playerOneReady = 0;
  playerTwoReady = 0;
  incomingByte = 0;
  readyScreen();
  while(!playerOneReady || !playerTwoReady) {
    if(Serial.available() > 0)
    {
      // Read the incoming byte
      incomingByte = Serial.read();

      if(incomingByte == '1' || incomingByte == 'w' || incomingByte == 'a' || incomingByte == 's' || incomingByte == 'd' || incomingByte == 'f' || incomingByte == 'g')
      {
        playerOneReady = 1;
        readyScreen();
      }
      if(incomingByte == '2' || incomingByte == 'i' || incomingByte == 'j' || incomingByte == 'k' || incomingByte == 'l' || incomingByte == ';' || incomingByte == '\'')
      {
        playerTwoReady = 1;
        readyScreen();
      }
    }
  }
  
  while(Serial.available() > 0)
  {
    Serial.read();
  }

  delay(1000);
  gameStartScreen();
}


//
// winSound
//
// This function plays the win sound effect at the end of a game.
//
void winSound()
{
  // D5 F#5 A5
  tone(buzzerPin, 587, 200);
  delay(100);
  tone(buzzerPin, 740, 200);
  delay(100);
  tone(buzzerPin, 880, 200);
  delay(100);
  noTone(buzzerPin);
}


//
// tieSound
//
// This function plays the tie sound effect at the end of a game, when neither player wins.
//
void tieSound() 
{
  // D5 C5 B5
  tone(buzzerPin, 587, 200);
  delay(100);
  tone(buzzerPin, 523, 200);
  delay(100);
  tone(buzzerPin, 494, 200);
  delay(100);
  noTone(buzzerPin);
}


// 
// readyScreen
//
// This function draws the ready screen for the game. It will be called only when one of the players indicate that they
// are currently ready.
//
void readyScreen() 
{
  tft.fillScreen(BLACK);
  tft.setCursor(cpuX - 55, cpuY);
  tft.setTextSize(2);
  tft.print("Ready Up!");

  tft.fillCircle(playerOneX, playerOneY, 40, playerOneColor);
  tft.setCursor(playerOneX, playerOneY);
  if (playerOneReady) {
    tft.setTextSize(2);
    tft.print("R!");
  }
  tft.fillCircle(playerTwoX, playerTwoY, 40, playerOneColor);
  tft.setCursor(playerTwoX, playerTwoY);
  if (playerTwoReady) {
    tft.setTextSize(2);
    tft.print("R!");
  }  
}


//
// gameStartScreen
//
// This function draws the game start screen for the game.
// It will be drawn at the start of each game once both players
// are ready.
//
void gameStartScreen() 
{
  tft.fillScreen(BLUE);
  tft.setCursor(cpuX - 60, cpuY);
  tft.setTextSize(2);
  tft.print("Game Starting!");

  tft.fillCircle(playerOneX, playerOneY, 40, playerOneColor);
  tft.setCursor(playerOneX, playerOneY);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print("R!");
  
  tft.fillCircle(playerTwoX, playerTwoY, 40, playerOneColor);
  tft.setCursor(playerTwoX, playerTwoY);
  tft.setTextSize(2);
  tft.print("R!");
  delay(1000);
  incomingByte = 0;
}


//
// winScreen
//
// This function draws the win screen for the game.
// 
void winScreen()
{
  tft.fillScreen(0x07F2);
  tft.setCursor(50, screenHeight / 2 - 5);
  tft.setTextColor(BLACK);
  tft.setTextSize(3);
  if(playerTwoLives == 0)  
    tft.print("Player 1 Wins!");
  if(playerOneLives == 0)
    tft.print("Player 2 Wins!");
}


//
// tieScreen
//
// This function draws the tie screen for the game, when neither player wins the game.
// 
void tieScreen()
{
  tft.fillScreen(0xFEE0);
  tft.setCursor(75, screenHeight / 2 - 5);
  tft.setTextColor(BLACK);
  tft.setTextSize(3);
  tft.print("Tie Game!");
}


//
// draw
//
// This function draws the newest state of the game whenever called, such as the
// the state of players, CPU, and live total.
// 
void draw()
{
  tft.fillScreen(BLACK);    

  drawPlayerOne();
  drawPlayerTwo();
  drawLives();
  drawCPU();
}


//
// getPlayerOneInput
//
// This function uses the incomingByte value from the controller Arduino to check if player one's state should
// be changed.
// 
void getPlayerOneInput()
{
  if (playerOneState == IDLE && (millis() - roundStart < roundTime) && playerTwoResult != CORRECT) {
    if(incomingByte == 'w')
    {
      playerOneColor = RED;
      playerOneState = UP;
      gameLogic();
      draw();
    }
    if(incomingByte == 'a')
    {
      playerOneColor = BLUE;
      playerOneState = LEFT;
      gameLogic();
      draw();
    }
    if(incomingByte == 's')
    {
      playerOneColor = GREEN;
      playerOneState = DOWN;
      gameLogic();
      draw();
    }
    if(incomingByte == 'd')
    {
      playerOneColor = YELLOW;
      playerOneState = RIGHT;
      gameLogic();
      draw();
    }
    if(incomingByte == 'f')
    {
      playerOneColor = CYAN;
      playerOneState = BUTTON;
      gameLogic();
      draw();
    }
    if(incomingByte == 'g')
    {
      playerOneColor = WHITE;
      playerOneState = FAKE;
      gameLogic();
    }
  }
}


//
// getPlayerTwoInput
//
// This function uses the incomingByte value from the controller Arduino to check if player two's state should
// be changed.
// 
void getPlayerTwoInput()
{
  if (playerTwoState == IDLE && (millis() - roundStart < roundTime) && playerOneResult != CORRECT) {
    if(incomingByte == 'i')
    {
      playerTwoColor = RED;
      playerTwoState = UP;
      gameLogic();
      draw();
    }
    if(incomingByte == 'j')
    {
      playerTwoColor = BLUE;
      playerTwoState = LEFT;
      gameLogic();
      draw();
    }
    if(incomingByte == 'k')
    {
      playerTwoColor = GREEN;
      playerTwoState = DOWN;
      gameLogic();
      draw();
    }
    if(incomingByte == 'l')
    {
      playerTwoColor = YELLOW;
      playerTwoState = RIGHT;
      gameLogic();
      draw();
    } 
    if(incomingByte == ';')
    {
      playerTwoColor = CYAN;
      playerTwoState = BUTTON;
      gameLogic();
      draw();
    }
    if(incomingByte == '\'')
    {
      playerTwoColor = WHITE;
      playerTwoState = FAKE;
      gameLogic();
    }
  }
}


//
// timeOut
//
// This function will check the states of the players when the current round's
// timer runs out.
//
void timeOut() {
    if (playerOneResult != CORRECT && playerOneResult != LIVELOST && playerOneResult != LOST) {
      if (playerOneLives <= 1) {  // Player 1 lost
        playerOneResult = LOST;
      } 
      else {  // Player 1 loses a life
        playerOneResult = LIVELOST;
      }
      playerOneLives--;
    }

    if (playerTwoResult != CORRECT && playerTwoResult != LIVELOST && playerTwoResult != LOST) {
      if (playerTwoLives <= 1) {  // Player 2 lost
        playerTwoResult = LOST;
      } 
      else {  // Player 2 loses a life
        playerTwoResult = LIVELOST;
      }
      playerTwoLives--;
    }

    if (playerOneResult == LOST && playerTwoResult == LOST) {
        playerOneResult = DRAW;
        playerTwoResult = DRAW;
    }
}

//
// gameLogic
//
// This function will hold the game logic for the game; it uses the state of the players, as well as the state 
// of the leader CPU, to determine if a player has lost, if a player lost a live, or if the game is continuing.
//
void gameLogic() {
    //
    // Player 1 has made a move
    //
      if (playerOneState != IDLE && playerOneState != FAKE && playerOneResult != LOST && playerOneResult != LIVELOST && playerOneResult != CORRECT) {
        if (playerOneState != cpuState) {  // Player 1 made a wrong move
          if (playerOneLives <= 1) {  // Player 1 lost
            playerOneResult = LOST;
          } 
          else {  // Player 1 loses a life
            playerOneResult = LIVELOST;
          }
          playerOneLives--;
        }
        else  // Player 1 made the correct move
        {
          playerOneResult = CORRECT;
        }
      }
      else if (playerOneState == FAKE && cpuState == IDLE && playerOneFake)
      {
          roundTime += fakeDelay;
          cpuDelay += fakeDelay;
          playerOneFake = 0;
          playerOneState = IDLE;
          fakeStart = millis();
          
          cpuColor = FAKECOLOR;
          cpuState = random(FAKERIGHT, FAKEBUTTON + 1);
          draw();
      }
    
    
    else if (playerOneState == FAKE)  // playerOneState is fake, but player has no more fake plays (playerOneFake <= 0)
    {
        playerOneState = IDLE;
    }
    
    
    //
    // Player 2 has made a move
    //
      if (playerTwoState != IDLE && playerTwoState != FAKE && playerTwoResult != LOST && playerTwoResult != LIVELOST && playerTwoResult != CORRECT) {
        if (playerTwoState != cpuState) {  // Player 2 made a wrong move
          if (playerTwoLives <= 1) {  // Player 2 lost
            playerTwoResult = LOST;
          } 
          else {  // Player 2 loses a life
            playerTwoResult = LIVELOST;
          }
          playerTwoLives--;
        }
        else  // Player 1 made the correct move
        {
          playerTwoResult = CORRECT;
        }
      }
      else if (playerTwoState == FAKE && cpuState == IDLE && playerTwoFake)
      {
          roundTime += fakeDelay;
          cpuDelay += fakeDelay;
          playerTwoFake = 0;
          playerTwoState = IDLE;
          fakeStart = millis();
          
          cpuColor = FAKECOLOR;
          cpuState = random(FAKERIGHT, FAKEBUTTON + 1);
          draw(); 
      }
    
    
    else if (playerTwoState == FAKE)  // playerOneState is fake, but player has no more fake plays (playerOneFake <= 0)
    {
        playerTwoState = IDLE;
    }
    
}

//
// newRound
//
// This function will initialize a new round of the game.
//
void newRound() {
  cpuDelay = random(minCPUDelay, maxCPUDelay);
  playerOneState = IDLE;
  playerOneResult = NONE;
  playerOneColor = WHITE;
  playerTwoState = IDLE;
  playerTwoResult = NONE;
  playerTwoColor = WHITE;
  cpuState = IDLE;
  cpuColor = WHITE;
  draw();
  roundStart = millis();
  roundTime = cpuDelay + timer;
}


//
// resetGame
//
// This function resets the entire game, including resetting players values.
//
void resetGame() {
  playerOneLives = 3;
  playerTwoLives = 3;
  playerOneFake = 1;
  playerTwoFake = 1;
  playerOneColor = WHITE;
  playerTwoColor = WHITE;
  readyUp();
}

void setup() {
  incomingByte = 0;

  Serial.begin(9600);  // start serial for output
  uint16_t ID = tft.readID();

  // Buzzer
  pinMode(buzzerPin, OUTPUT);

  // Screen 
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLUEMAIN);
  screenWidth = tft.width();
  screenHeight = tft.height();

  // Start screen
  startScreen();
  delay(startScreenDelay);
  
  // Game positions
  playerOneX = screenWidth / 4;
  playerOneY = screenHeight * 2 / 3;
  playerTwoX = screenWidth * 3 / 4;
  playerTwoY = screenHeight * 2 / 3;
  cpuX = screenWidth / 2;
  cpuY = 60;
  
  while(Serial.available() > 0)
    Serial.read();

  resetGame();
  newRound();
}

void loop() {
  // Keyboard input

  if(Serial.available() > 0)
  {
    // Read the incoming byte
    incomingByte = Serial.read();

    
  }

  if(millis() - roundStart > roundTime)
  { 
    timeOut();
    delay(downTime);  // TODO: Add an indication that the leader is no longer taking commands from players

    if(playerOneLives == 0 && playerTwoLives == 0)
    {
      tieScreen();
      tieSound();
      for(int i = 0; i < 10; i++)
        tieScreen();
      delay(resultScreenDelay);
      
      // Show start screen again
      tft.fillScreen(BLUEMAIN);
      startScreen();
      delay(startScreenDelay);
      while(Serial.available() > 0)
      {
        Serial.read();
      }
      
      resetGame();  
    }
    else if(playerOneLives == 0 || playerTwoLives == 0)
    {
      winScreen();
      winSound();
      for(int i = 0; i < 10; i++)
        winScreen();
      delay(resultScreenDelay);

      // Show start screen again
      tft.fillScreen(BLUEMAIN);
      startScreen();
      delay(startScreenDelay);
      while(Serial.available() > 0)
      {
        Serial.read();
      }

      resetGame();
    }
      
    newRound();
    while(Serial.available() > 0)
      Serial.read();      
    
    incomingByte = 0;
  }

  // the CPU's state is now changed
  if((millis() - roundStart > roundTime - timer) && cpuState == IDLE)
  {
    cpuState = random(RIGHT, BUTTON + 1);
    draw();
  }

  // the CPU is no longer fake
  if((millis() - fakeStart > fakeDelay) && (cpuState >= FAKEUP && cpuState <= FAKEBUTTON))
  {
    cpuState = IDLE;
    cpuColor = WHITE;
    draw();
  }
    
  getPlayerOneInput();
  getPlayerTwoInput();

  incomingByte = 0;
}
