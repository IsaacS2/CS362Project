// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
//
// This demo should work with most Adafruit TFT libraries
// If you are not using a shield,  use a full Adafruit constructor()
// e.g. Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

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


#include <Wire.h>

// Screen
int screenWidth;
int screenHeight;

// Input
int incomingByte = 0;

// Player 1
int playerOneState = IDLE;
int playerOneResult = NONE;
uint16_t playerOneColor = WHITE;
int playerOneX;
int playerOneY;
int playerOneLives;
int playerOneFake = 1;

// Player 2
int playerTwoState = IDLE;
int playerTwoResult = NONE;
uint16_t playerTwoColor = WHITE;
int playerTwoX;
int playerTwoY;
int playerTwoLives;
int playerTwoFake = 1;

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
unsigned long maxCPUDelay = 3000;  // maximum time before CPU
unsigned long cpuDelay;
unsigned long roundTime;
unsigned long downTime = 2000;
unsigned long fakeDelay = 2000;
unsigned long roundStart;
unsigned long resultScreenDelay = 5000;

// cpuDelay = random(maxCPUDelay)
// Round time = cpuDelay + timer
// roundTime -> downTime -> ...

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
    tft.fillCircle(cpuX, cpuY, 40, BLACK);
  }
}

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

void tieScreen()
{
  tft.fillScreen(0xFEE0);
  tft.setCursor(75, screenHeight / 2 - 5);
  tft.setTextColor(BLACK);
  tft.setTextSize(3);
  tft.print("Tie Game!");
}

void draw()
{
  tft.fillScreen(BLACK);    

  drawPlayerOne();
  drawPlayerTwo();
  drawLives();
  drawCPU();
}


void getPlayerOneInput()
{
  if (playerOneState == IDLE && (millis() - roundStart < roundTime) ) {
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
      //draw();
    }
  }
}

void getPlayerTwoInput()
{
  if (playerTwoState == IDLE && (millis() - roundStart < roundTime) ) {
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
      //draw();
    }
  }
}

//
// timeOut
//
// This function will check the states of the players if the current game's
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
// of the Simon Says CPU, to determine if a player has lost, if a player lost a live, or if the game is continuing.
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
    
}

//
// newRound
//
// This function will initialize a new round of the game
//
void newRound() {
  cpuDelay = random(0, maxCPUDelay);
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
// This function resets the entire game, including resetting players values
//
void resetGame() {
  playerOneLives = 3;
  playerTwoLives = 3;
  playerOneFake = 1;
  playerTwoFake = 1;
}

void setup() {
  Serial.begin(9600);  // start serial for output
  uint16_t ID = tft.readID();

  // Screen 
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  screenWidth = tft.width();
  screenHeight = tft.height();

  // Game positions
  playerOneX = screenWidth / 4;
  playerOneY = screenHeight * 2 / 3;
  playerTwoX = screenWidth * 3 / 4;
  playerTwoY = screenHeight * 2 / 3;
  cpuX = screenWidth / 2;
  cpuY = 60;

  resetGame();
  draw();
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
    delay(downTime);

    if(playerOneLives == 0 && playerTwoLives == 0)
    {
      for(int i = 0; i < 10; i++)
        tieScreen();
      delay(resultScreenDelay);
      resetGame();
      
    }
    else if(playerOneLives == 0 || playerTwoLives == 0)
    {
      for(int i = 0; i < 10; i++)
        winScreen();
      delay(resultScreenDelay);
      resetGame();
      
    }
    
    newRound();
    while(Serial.available() > 0)
      Serial.read();      
    incomingByte = 0;

  }

  // the CPU's state is now changed
  if((millis() - roundStart > roundTime - timer) && cpuState == IDLE )
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

  incomingByte = 0; // Clear Input
  
}
