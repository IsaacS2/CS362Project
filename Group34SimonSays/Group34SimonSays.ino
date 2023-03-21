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

#include <Wire.h>

// Screen
int screenWidth;
int screenHeight;

// Input
int incomingByte = 0;

// Player 1
int playerOneState = 0;
uint16_t playerOneColor = WHITE;
int playerOneX;
int playerOneY;
int playerOneLives;

// Player 2
int playerTwoState = 0;
uint16_t playerTwoColor = WHITE;
int playerTwoX;
int playerTwoY;
int playerTwoLives;

// CPU
int cpuState = 0;
uint16_t cpuColor = WHITE;
int cpuX;
int cpuY;

// Sizes
int arrowSize = 15;
int cpuArrowSize = 25;
int lifeSize = 10;

void drawPlayerOne()
{
  tft.fillCircle(playerOneX, playerOneY, 40, playerOneColor);

  if(playerOneState == 1)
  {
    tft.fillTriangle(playerOneX - arrowSize, playerOneY + arrowSize, playerOneX + arrowSize, playerOneY + arrowSize, playerOneX, playerOneY - arrowSize, BLACK);     
  }
  if(playerOneState == 2)
  {
    tft.fillTriangle(playerOneX + arrowSize, playerOneY - arrowSize, playerOneX + arrowSize, playerOneY + arrowSize, playerOneX - arrowSize, playerOneY, BLACK);     
  }
  if(playerOneState == 3)
  {
    tft.fillTriangle(playerOneX - arrowSize, playerOneY - arrowSize, playerOneX + arrowSize, playerOneY - arrowSize, playerOneX, playerOneY + arrowSize, BLACK);     
  }  
  if(playerOneState == 4)
  {
    tft.fillTriangle(playerOneX - arrowSize, playerOneY - arrowSize, playerOneX - arrowSize, playerOneY + arrowSize, playerOneX + arrowSize, playerOneY, BLACK);     
  }  
}

void drawPlayerTwo()
{
  tft.fillCircle(playerTwoX, playerTwoY, 40, playerTwoColor);

  if(playerTwoState == 1)
  {
    tft.fillTriangle(playerTwoX - arrowSize, playerTwoY + arrowSize, playerTwoX + arrowSize, playerTwoY + arrowSize, playerTwoX, playerTwoY - arrowSize, BLACK);     
  }
  if(playerTwoState == 2)
  {
    tft.fillTriangle(playerTwoX + arrowSize, playerTwoY - arrowSize, playerTwoX + arrowSize, playerTwoY + arrowSize, playerTwoX - arrowSize, playerTwoY, BLACK);     
  }
  if(playerTwoState == 3)
  {
    tft.fillTriangle(playerTwoX - arrowSize, playerTwoY - arrowSize, playerTwoX + arrowSize, playerTwoY - arrowSize, playerTwoX, playerTwoY + arrowSize, BLACK);     
  }  
  if(playerTwoState == 4)
  {
    tft.fillTriangle(playerTwoX - arrowSize, playerTwoY - arrowSize, playerTwoX - arrowSize, playerTwoY + arrowSize, playerTwoX + arrowSize, playerTwoY, BLACK);     
  }

}

void drawCPU()
{
  tft.fillCircle(cpuX, cpuY, 50, cpuColor);

  if(cpuState == 1)
  {
    tft.fillTriangle(cpuX - cpuArrowSize, cpuY + cpuArrowSize, cpuX + cpuArrowSize, cpuY + cpuArrowSize, cpuX, cpuY - cpuArrowSize, ~cpuColor);     
  }
  if(cpuState == 2)
  {
    tft.fillTriangle(cpuX + cpuArrowSize, cpuY - cpuArrowSize, cpuX + cpuArrowSize, cpuY + cpuArrowSize, cpuX - cpuArrowSize, cpuY, ~cpuColor);     
  }
  if(cpuState == 3)
  {
    tft.fillTriangle(cpuX - cpuArrowSize, cpuY - cpuArrowSize, cpuX + cpuArrowSize, cpuY - cpuArrowSize, cpuX, cpuY + cpuArrowSize, ~cpuColor);     
  }  
  if(cpuState == 4)
  {
    tft.fillTriangle(cpuX - cpuArrowSize, cpuY - cpuArrowSize, cpuX - cpuArrowSize, cpuY + cpuArrowSize, cpuX + cpuArrowSize, cpuY, ~cpuColor);     
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
  if(incomingByte == 'w')
  {
    playerOneColor = RED;
    playerOneState = 1;
    draw();
  }
  if(incomingByte == 'a')
  {
    playerOneColor = BLUE;
    playerOneState = 2;
    draw();
  }
  if(incomingByte == 's')
  {
    playerOneColor = GREEN;
    playerOneState = 3;
    draw();
  }
  if(incomingByte == 'd')
  {
    playerOneColor = YELLOW;
    playerOneState = 4;
    draw();
  }
  if(incomingByte == 'f')
  {
    playerOneColor = CYAN;
    playerOneState = 5;
    draw();
  }
  if(incomingByte == 'g')
  {
    playerOneColor = MAGENTA;
    playerOneState = 6;
    draw();
  }
  
}

void getPlayerTwoInput()
{
  if(incomingByte == 'i')
  {
    playerTwoColor = RED;
    playerTwoState = 1;
    draw();
  }
  if(incomingByte == 'j')
  {
    playerTwoColor = BLUE;
    playerTwoState = 2;
    draw();
  }
  if(incomingByte == 'k')
  {
    playerTwoColor = GREEN;
    playerTwoState = 3;
    draw();
  }
  if(incomingByte == 'l')
  {
    playerTwoColor = YELLOW;
    playerTwoState = 4;
    draw();
  } 
  if(incomingByte == ';')
  {
    playerTwoColor = CYAN;
    playerTwoState = 5;
    draw();
  }
  if(incomingByte == '\'')
  {
    playerTwoColor = MAGENTA;
    playerTwoState = 6;
    draw();
  }

}

void setup() {
  Serial.begin(9600);  // start serial for output
  // uint16_t ID = tft.readID();

  // Screen 
  tft.begin();
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

  // Game values
  playerOneLives = 3;
  playerTwoLives = 3;
  draw();
}

void loop() {
  // Keyboard input
  if(Serial.available() > 0)
  {
    // Read the incoming byte
    incomingByte = Serial.read();
  }

  getPlayerOneInput();
  getPlayerTwoInput();

  incomingByte = 0; // Clear Input
  
  // tft.drawChar(200, 50, incomingByte, YELLOW, BLACK, 2);
  
}