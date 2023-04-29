// Isaac Sanchez, Sebastian Ho
// References for code: https://iot-guider.com/arduino/serial-communication-between-two-arduino-boards/
                        https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
                        https://www.arduino.cc/reference/en/language/functions/communication/serial/available/

// Player 1 controls
// Joystick 1 pins
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



// Player 2 controls
// Joystick 2 pins
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


char p1SendByte = '1';
char p2SendByte = '2';

void setup() {
  pinMode(p1LeftButtonPin, INPUT);  
  pinMode(p1RightButtonPin, INPUT);
  pinMode(p1SendButtonPin, INPUT);

  pinMode(p2LeftButtonPin, INPUT);  
  pinMode(p2RightButtonPin, INPUT);
  pinMode(p2SendButtonPin, INPUT);
  
  Serial.begin(9600);
}


void loop() {
  // Read values for p1
  p1xValue = analogRead(p1X_pin);
  p1yValue = analogRead(p1Y_pin);
  
  p1LeftButtonStateNew = digitalRead(p1LeftButtonPin);
  p1RightButtonStateNew = digitalRead(p1RightButtonPin);
  p1SendButtonStateNew = digitalRead(p1SendButtonPin);

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

  //
  // Toggle code from https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
  //
  if(p1LeftButtonStateOld == 0 && p1LeftButtonStateNew == 1)
  {
    if(p1LeftButtonStateReading == 0)
    {
      p1SendByte = 'f';
      p1LeftButtonStateReading = 1;
    }
    else if(p1LeftButtonStateReading == 1)
    {
      p1SendByte = 'f';
      p1LeftButtonStateReading = 0;
    }
  }    

  p1LeftButtonStateOld = p1LeftButtonStateNew;

  //
  // Toggle code from https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
  //
  if(p1RightButtonStateOld == 0 && p1RightButtonStateNew == 1)
  {
    if(p1RightButtonStateReading == 0)
    {
      p1SendByte = 'g';
      p1RightButtonStateReading = 1;
    }
    else if(p1LeftButtonStateReading == 1)
    {
      p1SendByte = 'g';
      p1RightButtonStateReading = 0;
    }
  }    

  p1RightButtonStateOld = p1RightButtonStateNew;

  //
  // Toggle code from https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
  //
  if(p1SendButtonStateOld == 0 && p1SendButtonStateNew == 1)
  {
    if(p1SendButtonStateReading == 0)
    {
      Serial.print(p1SendByte);  // serial communication inspired by https://iot-guider.com/arduino/serial-communication-between-two-arduino-boards/ and https://www.arduino.cc/reference/en/language/functions/communication/serial/available/
      p1SendButtonStateReading = 1;
    }
    else if(p1SendButtonStateReading == 1)
    {
      Serial.print(p1SendByte);  // serial communication inspired by https://iot-guider.com/arduino/serial-communication-between-two-arduino-boards/ and https://www.arduino.cc/reference/en/language/functions/communication/serial/available/
      p1SendButtonStateReading = 0;
    }
  }    

  p1SendButtonStateOld = p1SendButtonStateNew;
  

  // Read values for p2
  p2xValue = analogRead(p2X_pin);
  p2yValue = analogRead(p2Y_pin);

  p2LeftButtonStateNew = digitalRead(p2LeftButtonPin);
  p2RightButtonStateNew = digitalRead(p2RightButtonPin);
  p2SendButtonStateNew = digitalRead(p2SendButtonPin);


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

  //
  // Toggle code from https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
  //
  if(p2LeftButtonStateOld == 0 && p2LeftButtonStateNew == 1)
  {
    if(p2LeftButtonStateReading == 0)
    {
      p2SendByte = ';';
      p2LeftButtonStateReading = 1;
    }
    else if(p2LeftButtonStateReading == 1)
    {
      p2SendByte = ';';
      p2LeftButtonStateReading = 0;
    }
  }    

  p2LeftButtonStateOld = p2LeftButtonStateNew;

  //
  // Toggle code from https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
  //
  if(p2RightButtonStateOld == 0 && p2RightButtonStateNew == 1)
  {
    if(p2RightButtonStateReading == 0)
    {
      p2SendByte = '\'';
      p2RightButtonStateReading = 1;
    }
    else if(p2RightButtonStateReading == 1)
    {
      p2SendByte = '\'';
      p2RightButtonStateReading = 0;
    }
  }    

  p2RightButtonStateOld = p2RightButtonStateNew;

  //
  // Toggle code from https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
  //
  if(p2SendButtonStateOld == 0 && p2SendButtonStateNew == 1)
  {
    if(p2SendButtonStateReading == 0)
    {
      Serial.print(p2SendByte);  // serial communication inspired by https://iot-guider.com/arduino/serial-communication-between-two-arduino-boards/ and https://www.arduino.cc/reference/en/language/functions/communication/serial/available/
      p2SendButtonStateReading = 1;
    }
    else if(p2SendButtonStateReading == 1)
    {
      Serial.print(p2SendByte);  // serial communication inspired by https://iot-guider.com/arduino/serial-communication-between-two-arduino-boards/ and https://www.arduino.cc/reference/en/language/functions/communication/serial/available/
      p2SendButtonStateReading = 0;
    }
  }    

  p2SendButtonStateOld = p2SendButtonStateNew;
}
