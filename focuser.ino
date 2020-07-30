// include Arduino stepper motor library
#include <Stepper.h>
 
// define number of steps per revolution
#define STEPS 32
 
// define stepper motor control pins
#define IN1  10//11
#define IN2  11//10
#define IN3  12 //9
#define IN4  13 //8


int speedRedVal = 2; // Your value of choice, 0.25, 0.5 1.5 etc

// initialize stepper library
Stepper stepper(STEPS, IN4, IN2, IN3, IN1);
 
// joystick pot output is connected to Arduino A0
#define joystick  A0

// this constant won't change:
const int  buttonPin = 15;    // the joystick SW pin that the pushbutton is attached to

bool speedFlag = true;
// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
 
void setup()
{
  //Serial.begin(9600);// NOTE the output serial coul reduce the speed motor if is begin, only use it for debuging
   pinMode(buttonPin, INPUT_PULLUP);  //Init SW pin with Pull-up internal
}
 
void loop()
{
//Change state detection fr SW pin
 buttonState = digitalRead(buttonPin);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      if (speedFlag == false) {
      speedFlag = true;
      //Serial.println("False");
    } 
    else {
      speedFlag = false;
      //Serial.println("True");
    }
    } 
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  // read analog value from the potentiometer
  int val = analogRead(joystick);
 
  // if the joystic is in the middle ===> stop the motor
  if(  (val > 500) && (val < 523) )
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
 
  else
  {
    // move the motor in the first direction
    while (val >= 523)
    {
      // map the speed between 5 and 500 rpm
      if (speedFlag == true){// if value true then reduce by speedRedVal
        val = val -((val-523)/ speedRedVal); 
      }
      //Serial.println(val);
      int speed_ = map(val, 523, 1023, 5, 500);
      // set motor speed
      stepper.setSpeed(speed_);
 
      // move the motor (1 step)
      stepper.step(1);
 
      val = analogRead(joystick);
    }
 
    // move the motor in the other direction
    while (val <= 500)
    {
      // map the speed between 5 and 500 rpm
            if (speedFlag == true){// if value true then reduce by speedRedVal
        val = val +((500-val)/ speedRedVal); 
      }
      //Serial.println(val);
      int speed_ = map(val, 500, 0, 5, 500);
      // set motor speed
      stepper.setSpeed(speed_);
 
      // move the motor (1 step)
      stepper.step(-1);
 
      val = analogRead(joystick);
    }
 
  }
 
}
