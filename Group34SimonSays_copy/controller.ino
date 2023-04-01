// Isaac Sanchez, Sebastian Ho

// I2C connection
#include <Wire.h>


// Player 1 controls
// Joystick 1 pins
// const int SW_pin = 8; // digital pin connected to switch
// int bValue = 0 ; // value of the button reading	 
const int p1X_pin = A1; // analog pin connected to X output
const int p1Y_pin = A2; // analog pin connected to Y output
int p1xValue = 0 ; // read value of the X axis	
int p1yValue = 0 ; // read value of the Y axis	

const int p1LeftButtonPin = 2;
int p1LeftButtonStateOld;
int p1LeftButtonStateNew;
int p1LeftButtonStateReading;

const int p1RightButtonPin = 3;
int p1RightButtonStateOld;
int p1RightButtonStateNew;
int p1RightButtonStateReading;

int p1LeftDebounceTime = 0;
int p1RightDebounceTime = 0;

// Player 2 controls
// Joystick 2 pins
// const int SW_pin = 8; // digital pin connected to switch
// int bValue = 0 ; // value of the button reading	 
const int p2X_pin = A3; // analog pin connected to X output
const int p2Y_pin = A4; // analog pin connected to Y output
int p2xValue = 0 ; // read value of the X axis	
int p2yValue = 0 ; // read value of the Y axis	

const int p2LeftButtonPin = 2;
int p2LeftButtonStateOld;
int p2LeftButtonStateNew;
int p2LeftButtonStateReading;

const int p2RightButtonPin = 3;
int p2RightButtonStateOld;
int p2RightButtonStateNew;
int p2RightButtonStateReading;

int p2LeftDebounceTime = 0;
int p2RightDebounceTime = 0;

int debounceDelay = 20;
char p1SendByte = 0;
char p2SendByte = 0;

void requestEvent() {
  char *sendByte = calloc(2, sizeof(char));

  sendByte[0] = p1SendByte;
  sendByte[1] = p2SendByte;

  Wire.write(sendByte);

  free(sendByte);
}

void setup() {
  
  Wire.begin(6); // The controller has address 6
  // Serial.begin(9600);
  Wire.onRequest(requestEvent);

}

void loop() {
  // Read values for p1
  p1xValue = analogRead(p1X_pin);	
	p1yValue = analogRead(p1Y_pin);
  p1LeftStateReading = digitalRead(p1LeftButtonPin);
  p1RightStateReading = digitalRead(p1RightButtonPin);
  

  buttonStateReading = digitalRead(buttonPin);

  // p1 Right
  if(p1xValue > 800)
  {
    p1SendByte = 'd';
  }

  // p1 Left
  if(p1xValue < 200)
  {
    p1SendByte = 'a';
  }

  // p1 Up
  if(p1yValue > 800)
  {
    p1SendByte = 'w';
  }

  // p1 Down
  if(p1yValue < 200)
  {
    p1SendByte = 's';
  }

  // p1 left button
  // If input changed from what it was before, update the debounce time
  if(p1LeftButtonStateOld != p1LeftButtonStateReading)
  {
    debounceTime = millis();
  }

  // Check if the change in button state was longer than the debounce delay
  if((millis() - p1LeftDebounceTime) > debounceDelay)
  {
    // If the current button state hasn't been updated, update it
    if(p1LeftButtonStateNew != p1LeftButtonStateReading)
    {
      p1LeftButtonStateNew = p1LeftButtonStateReading;  
      // Now check if the new button state is pressed down
      if(p1LeftButtonStateNew == HIGH)
      {
        p1SendByte = 'f';
      }    
    }
  }

  p1LeftButtonStateOld = p1LeftButtonStateReading;

  // p1 right button
  // If input changed from what it was before, update the debounce time
  if(p1RightButtonStateOld != p1RightButtonStateReading)
  {
    debounceTime = millis();
  }

  // Check if the change in button state was longer than the debounce delay
  if((millis() - p1RightDebounceTime) > debounceDelay)
  {
    // If the current button state hasn't been updated, update it
    if(p1RightButtonStateNew != p1RightButtonStateReading)
    {
      p1RightButtonStateNew = p1RightButtonStateReading;  
      // Now check if the new button state is pressed down
      if(p1RightButtonStateNew == HIGH)
      {
        p1SendByte = 'g';
      }    
    }
  }

  p1RightButtonStateOld = p1RightButtonStateReading;

  // Read values for p2
  p2xValue = analogRead(p2X_pin);	
	p2yValue = analogRead(p2Y_pin);
  p2LeftStateReading = digitalRead(p2LeftButtonPin);
  p2RightStateReading = digitalRead(p2RightButtonPin);
  

  buttonStateReading = digitalRead(buttonPin);

  // p2 Right
  if(p2xValue > 800)
  {
    p2SendByte = 'l';
  }

  // p2 Left
  if(p2xValue < 200)
  {
    p2SendByte = 'j';
  }

  // p2 Up
  if(p2yValue > 800)
  {
    p2SendByte = 'i';
  }

  // p2 Down
  if(p2yValue < 200)
  {
    p2SendByte = 'k';
  }

  // p2 left button
  // If input changed from what it was before, update the debounce time
  if(p2LeftButtonStateOld != p2LeftButtonStateReading)
  {
    debounceTime = millis();
  }

  // Check if the change in button state was longer than the debounce delay
  if((millis() - p2LeftDebounceTime) > debounceDelay)
  {
    // If the current button state hasn't been updated, update it
    if(p2LeftButtonStateNew != p2LeftButtonStateReading)
    {
      p2LeftButtonStateNew = p2LeftButtonStateReading;  
      // Now check if the new button state is pressed down
      if(p2LeftButtonStateNew == HIGH)
      {
        p2SendByte = ';';
      }    
    }
  }

  p2LeftButtonStateOld = p2LeftButtonStateReading;

  // p2 right button
  // If input changed from what it was before, update the debounce time
  if(p2RightButtonStateOld != p2RightButtonStateReading)
  {
    debounceTime = millis();
  }

  // Check if the change in button state was longer than the debounce delay
  if((millis() - p2RightDebounceTime) > debounceDelay)
  {
    // If the current button state hasn't been updated, update it
    if(p2RightButtonStateNew != p2RightButtonStateReading)
    {
      p2RightButtonStateNew = p2RightButtonStateReading;  
      // Now check if the new button state is pressed down
      if(p2RightButtonStateNew == HIGH)
      {
        p2SendByte = '\'';
      }    
    }
  }

  p2RightButtonStateOld = p2RightButtonStateReading;

}
