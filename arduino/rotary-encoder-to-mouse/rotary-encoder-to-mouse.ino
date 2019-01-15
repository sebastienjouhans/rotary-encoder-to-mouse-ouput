#include <Mouse.h>

#define encoder0PinA  2
#define encoder0PinB  3
#define pushButtonPin  4
#define maxEncoderValue  1024

int currentPushButtonState = 0;
int previousPushButtonState = 0;

float scrub = 0;
float encoderTick = 0;

volatile unsigned int currentEncoder0Pos = 0;
volatile unsigned int previouscurrentEncoder0Pos = 0;

long wheelValue = 0;

bool isWheelValuePositive = true;

void setup() {
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(pushButtonPin, INPUT_PULLUP);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Mouse.begin();

  Serial.begin (9600);
}

void loop() {
  
  updateButtonState();

  updateMouseWheel();
  
}

//void updateEncoderTick() {
//  if (currentEncoder0Pos < 0) {
//    scrub = 1 + (float(currentEncoder0Pos % maxEncoderValue) / float (maxEncoderValue)) ;
//  }
//  else {
//    scrub = float(currentEncoder0Pos % maxEncoderValue) / float (maxEncoderValue);
//  }
//  encoderTick = int((maxEncoderValue * scrub));
//  
//  Serial.println (scrub);
//  Serial.println (encoderTick);
//  Serial.println ("---");
//}

void updateMouseWheel()
{
  if(previouscurrentEncoder0Pos != currentEncoder0Pos)
  {
    if(currentEncoder0Pos > previouscurrentEncoder0Pos)
    {
      if(!isWheelValuePositive)
      {
        wheelValue = 0;
        isWheelValuePositive = true;
      }
      wheelValue++; 
    }
    else
    {
      if(isWheelValuePositive)
      {
        wheelValue = 0;
        isWheelValuePositive = false;
      }
      wheelValue--;
    }
    
    previouscurrentEncoder0Pos = currentEncoder0Pos;
  Serial.println (wheelValue);
    Mouse.move(0,0,wheelValue);
  }
}

void updateButtonState()
{
  currentPushButtonState = digitalRead(pushButtonPin);
  if(currentPushButtonState != previousPushButtonState)
  {
    previousPushButtonState = currentPushButtonState;
    Serial.println (currentPushButtonState, DEC);
    if (currentPushButtonState == HIGH) {
      Mouse.press();
    } else {
      Mouse.release();
    }
  }
}

void doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {
      currentEncoder0Pos = currentEncoder0Pos + 1;         // CW
    }
    else {
      currentEncoder0Pos = currentEncoder0Pos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == HIGH) {
      currentEncoder0Pos = currentEncoder0Pos + 1;          // CW
    }
    else {
      currentEncoder0Pos = currentEncoder0Pos - 1;          // CCW
    }
  }
  // use for debugging - remember to comment out
}

void doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {
      currentEncoder0Pos = currentEncoder0Pos + 1;         // CW
    }
    else {
      currentEncoder0Pos = currentEncoder0Pos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {
      currentEncoder0Pos = currentEncoder0Pos + 1;          // CW
    }
    else {
      currentEncoder0Pos = currentEncoder0Pos - 1;          // CCW
    }
  }
}



