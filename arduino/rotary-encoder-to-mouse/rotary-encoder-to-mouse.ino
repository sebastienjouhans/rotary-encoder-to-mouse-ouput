#include <Mouse.h>
#include <Keyboard.h>

#define encoder0PinA  2
#define encoder0PinB  3
#define pushButton1Pin  4
#define pushButton2Pin  5
#define maxEncoderValue  1024

char keyA = 'a';
char keyB = 'b';

int currentPushButton1State = 0;
int previousPushButton1State = 0;

int currentPushButton2State = 0;
int previousPushButton2State = 0;

float scrub = 0;
float encoderTick = 0;

volatile unsigned int currentEncoder0Pos = 0;
volatile unsigned int previouscurrentEncoder0Pos = 0;

long wheelValue = 0;

bool isWheelValuePositive = true;

void setup() {
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);

  pinMode(pushButton1Pin, INPUT_PULLUP);
  pinMode(pushButton2Pin, INPUT_PULLUP);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Mouse.begin();
  Keyboard.begin();

  Serial.begin (9600);
}

void loop() {
  
  updateButton1State();
  updateButton2State();

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

// output mousewheel positive value when going in one direction
// and output a negative value when going in the opposite direction
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

//ouput keyboard letter a when the button is pressed
void updateButton1State()
{
  currentPushButton1State = digitalRead(pushButton1Pin);
  if(currentPushButton1State != previousPushButton1State)
  {
    previousPushButton1State = currentPushButton1State;
    Serial.println (currentPushButton1State, DEC);
    if (currentPushButton1State == HIGH) {
      Keyboard.press(keyA);
    } else {
      Keyboard.release(keyA);
    }
  }
}

//ouput keyboard letter b when the button is pressed
void updateButton2State()
{
  currentPushButton2State = digitalRead(pushButton2Pin);
  if(currentPushButton2State != previousPushButton2State)
  {
    previousPushButton2State = currentPushButton2State;
    Serial.println (currentPushButton2State, DEC);
    if (currentPushButton2State == HIGH) {
      Keyboard.press(keyB);
    } else {
      Keyboard.release(keyB);
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



