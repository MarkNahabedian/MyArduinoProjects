/*
  SparkFun Inventor’s Kit
  Circuit 5C - Autonomous Robot

  This robot will drive around on its own and react to obstacles by backing up and turning to a new direction.
  This sketch was adapted from one of the activities in the SparkFun Guide to Arduino.
  Check out the rest of the book at
  https://www.sparkfun.com/products/14326

  This sketch was written by SparkFun Electronics, with lots of help from the Arduino community.
  This code is completely free for any use.

  View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v41
  Download drawings and code at: https://github.com/sparkfun/SIK-Guide-Code
*/

// Adding a potentiometer (wiper on pin A0) to control speed:
const int MAX_SPEED = 150;


//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 13;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 12;            //control pin 2 on the motor driver for the right motor
const int PWMA = 11;            //speed control pin on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 10;           //speed control pin on the motor driver for the left motor
const int BIN2 = 9;           //control pin 2 on the motor driver for the left motor
const int BIN1 = 8;           //control pin 1 on the motor driver for the left motor


//distance variables
const int trigPin = 6;
const int echoPin = 5;

const int RED_LED = 2;
const int GREEN_LED = 4;

int switchPin = 7;             //switch to turn the robot on and off

float distance = 0;            //variable to store the distance measured by the distance sensor

//robot behaviour variables
int backupTime = 300;           //amount of time that the robot will back up when it senses an object
int turnTime = 200;             //amount that the robot will turn once it has backed up

/********************************************************************************/
void setup()
{
  pinMode(trigPin, OUTPUT);       //this pin will send ultrasonic pulses out from the distance sensor
  pinMode(echoPin, INPUT);        //this pin will sense when the pulses reflect back to the distance sensor

  pinMode(switchPin, INPUT_PULLUP);   //set this as a pullup to sense whether the switch is flipped

  // LEDs to indicate the current activity in loop():
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.begin(9600);                       //begin serial communication with the computer
  Serial.print("To infinity and beyond!");  //test the serial connection
}

/********************************************************************************/

const int TURN_LIMIT = 10;
int turn = 0;
int turn_direction = 1;

void loop()
{
  //DETECT THE DISTANCE READ BY THE DISTANCE SENSOR
  distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" in");              // print the units

  if (digitalRead(switchPin) == LOW) { //if the on switch is flipped

    int pot = analogRead(A0);       // int 0-1023
    int speed = MAX_SPEED * (float(pot)/1023.0);

    if (distance < 10) {              //if an object is detected
      //back up and turn
      Serial.print(" ");
      Serial.println("BACK!");
      digitalWrite(RED_LED, HIGH);

      //stop for a moment
      rightMotor(0);
      leftMotor(0);
      delay(200);

      //back up
      rightMotor(-speed);
      leftMotor(-speed);
      delay(backupTime);

      //turn away from obstacle
      if (turn > TURN_LIMIT || turn < -TURN_LIMIT) {
        turn_direction = - turn_direction;
        Serial.print("turn_direction: ");
        Serial.println(turn_direction);
      }
      speed = speed * turn_direction;
      rightMotor(speed);
      leftMotor(-speed);
      turn += turn_direction;
      Serial.print("turn: ");
      Serial.println(turn);
      delay(turnTime);

    } else {                        //if no obstacle is detected drive forward
      Serial.println("Moving...");
      digitalWrite(GREEN_LED, HIGH);

      rightMotor(speed);
      leftMotor(speed);
    }
  } else {                        //if the switch is off then stop

    //stop the motors
    rightMotor(0);
    leftMotor(0);
  }

  delay(50);                      //wait 50 milliseconds between readings
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

/********************************************************************************/
void rightMotor(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void leftMotor(int motorSpeed)                        //function for driving the left motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}

