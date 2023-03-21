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

int incomingByte = 0;

// Player 1
int playerOneState = 0;
uint16_t playerOneColor = WHITE;

// Player 2
int playerTwoState = 0;
uint16_t playerTwoColor = WHITE;

void setup() {
  Serial.begin(9600);  // start serial for output
  // uint16_t ID = tft.readID();
    
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  screenWidth = tft.width();
  screenHeight = tft.height();
}

void loop() {
  // Keyboard input
  if(Serial.available() > 0)
  {
    // Read the incoming byte
    incomingByte = Serial.read();
  }

  // Player One
  if(incomingByte == 'w')
  {
    playerOneColor = RED;
  }
  if(incomingByte == 'a')
  {
    playerOneColor = BLUE;
  }
  if(incomingByte == 's')
  {
    playerOneColor = GREEN;
  }
  if(incomingByte == 'd')
  {
    playerOneColor = YELLOW;
  }
  if(incomingByte == 'f')
  {
    playerOneColor = CYAN;
  }
  if(incomingByte == 'g')
  {
    playerOneColor = MAGENTA;
  }

  // Player Two
  if(incomingByte == 'i')
  {
    playerTwoColor = RED;
  }
  if(incomingByte == 'j')
  {
    playerTwoColor = BLUE;
  }
  if(incomingByte == 'k')
  {
    playerTwoColor = GREEN;
  }
  if(incomingByte == 'l')
  {
    playerTwoColor = YELLOW;
  } 
  if(incomingByte == ';')
  {
    playerTwoColor = CYAN;
  }
  if(incomingByte == '\'')
  {
    playerTwoColor = MAGENTA;
  }

  // tft.drawChar(200, 50, incomingByte, YELLOW, BLACK, 2);
  tft.drawCircle(screenWidth / 2, 50, 50, MAGENTA);
  tft.drawCircle(screenWidth / 4, screenHeight * 2 / 3, 50, playerOneColor);
  tft.drawCircle(screenWidth * 3 / 4, screenHeight * 2 / 3, 50, playerTwoColor);
  
}