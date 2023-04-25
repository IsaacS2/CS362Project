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


const int p1SendButtonPin = 4;
int p1SendButtonStateOld;
int p1SendButtonStateNew;
int p1SendButtonStateReading;


int p1LeftDebounceTime = 0;
int p1RightDebounceTime = 0;
int p1SendDebounceTime = 0;


// Player 2 controls
// Joystick 2 pins
// const int SW_pin = 8; // digital pin connected to switch
// int bValue = 0 ; // value of the button reading  
const int p2X_pin = A3; // analog pin connected to X output
const int p2Y_pin = A4; // analog pin connected to Y output
int p2xValue = 0 ; // read value of the X axis 
int p2yValue = 0 ; // read value of the Y axis 


const int p2LeftButtonPin = 5;
int p2LeftButtonStateOld;
int p2LeftButtonStateNew;
int p2LeftButtonStateReading;


const int p2RightButtonPin = 6;
int p2RightButtonStateOld;
int p2RightButtonStateNew;
int p2RightButtonStateReading;


const int p2SendButtonPin = 7;
int p2SendButtonStateOld;
int p2SendButtonStateNew;
int p2SendButtonStateReading;


int p2LeftDebounceTime = 0;
int p2RightDebounceTime = 0;
int p2SendDebounceTime = 0;


int debounceDelay = 80;
char p1SendByte = '1';
char p2SendByte = '2';


char p1SendByteOld;
char p2SendByteOld;


// void requestEvent() {
//  // char *sendByte = calloc(2, sizeof(char));
//  // char sendByte[3];


//  // sendByte[0] = p1SendByte;
//  // sendByte[1] = p2SendByte;
//  // sendByte[2] = '\0';
//  // Serial.println(sendByte);
//  // Wire.write(sendByte, 2);
//  // Serial.println(p1SendByte);
//  // Serial.println(p2SendByte);
//  // Wire.write(p1SendByte);
//  // Wire.write(p2SendByte); 
//  // free(sendByte);


//  // TODO: SEND DATA FOR BOTH PLAYERS
//  // USE -1 TO REPRESENT WAITING ON DATA 
//  }


void setup() {
 Serial.begin(9600);
}


void loop() {
 // Read values for p1
 p1xValue = analogRead(p1X_pin);
 p1yValue = analogRead(p1Y_pin);
 p1LeftButtonStateReading = digitalRead(p1LeftButtonPin);
 p1RightButtonStateReading = digitalRead(p1RightButtonPin);
 p1SendButtonStateReading = digitalRead(p1SendButtonPin);


 // p1LeftButtonStateReading = digitalRead(p1buttonPin);


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
   p1LeftDebounceTime = millis();
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
   p1RightDebounceTime = millis();
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


 // p1 send button
 // If input changed from what it was before, update the debounce time
 if(p1SendButtonStateOld != p1SendButtonStateReading)
 {
   p1SendDebounceTime = millis();
 }


 // Check if the change in button state was longer than the debounce delay
 if((millis() - p1SendDebounceTime) > debounceDelay)
 {
   // If the current button state hasn't been updated, update it
   if(p1SendButtonStateNew != p1SendButtonStateReading)
   {
     p1SendButtonStateNew = p1SendButtonStateReading; 
     // Now check if the new button state is pressed down
     if(p1SendButtonStateNew == HIGH)
     {
       //p1SendByte = 'v';
       // TODO: SEND P1 DATA
       Serial.print(p1SendByte);
       //p1SendByte = 
     }   
   }
 }


 p1SendButtonStateOld = p1SendButtonStateReading;


 // Read values for p2
 p2xValue = analogRead(p2X_pin);
 p2yValue = analogRead(p2Y_pin);
 p2LeftButtonStateReading = digitalRead(p2LeftButtonPin);
 p2RightButtonStateReading = digitalRead(p2RightButtonPin);
 p2SendButtonStateReading = digitalRead(p2SendButtonPin);


 // buttonButtonStateReading = digitalRead(buttonPin);


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
   p2LeftDebounceTime = millis();
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
   p2RightDebounceTime = millis();
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


 // p2 send button
 // If input changed from what it was before, update the debounce time
 if(p2SendButtonStateOld != p2SendButtonStateReading)
 {
   p2SendDebounceTime = millis();
 }


 // Check if the change in button state was longer than the debounce delay
 if((millis() - p2SendDebounceTime) > debounceDelay)
 {
   // If the current button state hasn't been updated, update it
   if(p2SendButtonStateNew != p2SendButtonStateReading)
   {
     p2SendButtonStateNew = p2SendButtonStateReading; 
     // Now check if the new button state is pressed down
     if(p2SendButtonStateNew == HIGH)
     {
       // 
       //p2SendByte = 'b';

       // TODO: SEND P2 DATA
       Serial.print(p2SendByte);
     }   
   }
 }


 p2SendButtonStateOld = p2SendButtonStateReading;

 
}



