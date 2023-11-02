//Shawn Edwards//////////////////////////////////////////////////////////////////////////////////////
//10/20/2023/////////////////////////////////////////////////////////////////////////////////////////
//Microcontroller Project - Analog Joystick-Controlled Fan with Killswitch///////////////////////////

/* 
This project is intended to monitor and safeguard the operation of a DC motor.
Real world application would be industrial settings where large cooling fans are used. We want to be able to manually control as well as automatically control these fans.
In the case that one of these fans were to fall over, the safety mechanism of the tilt ball switch would shut the system down automatically.
In case of failure of the tilt ball switch, the analog joystick provides a killswitch as well.

I had more ambitious ideas for this project, but this is honestly my first time doing one of this scale so I hope this is satisfactory.
I used code from the LiquidCrystal library example codes for my pinout as well as my lcd initialization.
*/
#include <LiquidCrystal.h>

#define ENABLEFAN 9 //l293d enable pin
#define DIRA 1  //l293d input1
#define DIRB 10 //l293d input2
#define TILT_BALL 7 //tiltball switch pin
#define BUZZER 6  //passive buzzer pin
#define joySwitch 8 //analog joystick button switch pin

const int X_AXIS_PIN = A0; //analog joystick X pin
const int Y_AXIS_PIN = A1; //analog joystick Y pin
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //pinout for lcd

int pulseCycle = 0; //pulse cycle variable I used in my isPulseMode if statement to help with performance
int speed = 0; //speed initially 0
bool isPulseMode = HIGH; //pulse mode is default
bool tiltBallSwitchState = LOW; //tiltballswitch starts in LOW state

void setup() {

  //Declare our inputs and outputs
  pinMode(ENABLEFAN, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(TILT_BALL, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(joySwitch, INPUT_PULLUP);

  //initialize the lcd
  lcd.begin(16, 2);
  lcd.print("SPEED: ");
  lcd.setCursor(0, 1);
  lcd.print("MODE: ");
  
}

void loop() {
  int xValue;
  int yValue;
  static bool power = HIGH;
  // Read the tilt ball switch state
  tiltBallSwitchState = digitalRead(TILT_BALL);

  if (tiltBallSwitchState == HIGH || power == LOW) {
    digitalWrite(ENABLEFAN, LOW); // Turn off the fan
    lcd.setCursor(7, 0);
    lcd.print("OFF       ");
    lcd.setCursor(7,1);
    lcd.print("            ");
    if (analogRead(Y_AXIS_PIN) > 1000) {
      power = !power; // Toggle the power on and off
      tone(BUZZER, 2500, 50);
      delay(100);
      tone(BUZZER, 1000, 50);
      delay(250); // Debounce delay to avoid rapid toggling
    }
  } else {
    // Read the joystick values
    xValue = analogRead(X_AXIS_PIN);
    yValue = analogRead(Y_AXIS_PIN);
    if( yValue > 600)
    {
      power = !power;
    }


    if (digitalRead(joySwitch) == LOW) {
      isPulseMode = !isPulseMode; // Toggle pulse mode on analog joystick button press
      delay(250); // Delay to avoid spamming
      tone(BUZZER, 500, 50); //play a tone when button is pressed
      delay(50);
      tone(BUZZER, 1000, 50);
    }

    if (isPulseMode) {
      // Pulse Mode
      static bool pulse = LOW; 
      digitalWrite(DIRA, HIGH);
      digitalWrite(DIRB, LOW);
      //This is where I used pulseCycle variable. I found that if I delayed the pulse cycle with a delay function, the analog and digital read functions were not as responsive
      //In this implementation, every 10 cycles of the program, the pulse mode changes without responsiveness issues regarding the analog and digital read functions.
      if ((pulseCycle % 10) == 0)
      {
        pulse = !pulse;
      }
      speed = pulse ? 100 : 255; //choose speed of fan based on pulse variable state
      analogWrite(ENABLEFAN, speed);
      lcd.setCursor(7,1);
      lcd.print("PULSE      "); //print mode and speed to lcd
      lcd.setCursor(7, 0);
      lcd.print(speed);
      lcd.print("      ");
      pulseCycle++; //Increment pulseCycle variable, once it reaches a multiple of 10, the pulse variable changes states
    } else {
      // Manual Mode
      int Min = 300; // Minimum X value for exclusion
      int Max = 700; // Maximum X value for exclusion
      int speedChange = 0;

      if (xValue < Min) {
        speedChange = map(xValue, 0, Min, -10, 0);
        speed = constrain(speed + speedChange, 0, 255); // Constrain speed between 0 and 255
      } else if (xValue > Max) {
        speedChange = map(xValue, Max, 1023, 0, 10);
        speed = constrain(speed + speedChange, 0, 255); // Constrain speed between 0 and 255
      }

   
      

      digitalWrite(DIRA, HIGH); //Set fan direction; change as needed
      digitalWrite(DIRB, LOW);

      analogWrite(ENABLEFAN, speed);
      lcd.setCursor(7, 1);
      lcd.print("MANUAL "); //print mode and speed to lcd
      lcd.setCursor(7,0);
      lcd.print(speed);
      lcd.print("      ");
    }
 // }

    //Play tone when analog joystick exceeds X and Y thresholds respectively
    if (abs(xValue) > 700 || abs(yValue) > 700) {
      tone(BUZZER, 2500, 50);
      delay(100);
      tone(BUZZER, 1000, 50);
    }
    if (abs(xValue) < 300 || abs(yValue) < 300) {
      tone(BUZZER, 1500, 50);
      delay(100);
      tone(BUZZER, 1000, 50);
    }

    delay(100); //Delay just in case
  }
}
