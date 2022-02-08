/*
  Use the range finder and piezoelectric buzzer in the SparkFun
  Inventor's Kit to make a Theremin-line musical instrument.

  This uses code and circuitry from projects 2 and 3 of the Inventor's
  Kit.
*/


#include <math.h>

////////////////////////////////////////////////////////////
// Range finder:

const int trigPin = 11;           //connects to the trigger pin on the distance sensor
const int echoPin = 12;           //connects to the echo pin on the distance sensor

float getDistance() {
  // Emit 10ms pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float echoTime = pulseIn(echoPin, HIGH);
  return echoTime / 148.0;
}


////////////////////////////////////////////////////////////
// Playing tones using the piezo buzzer

const int buzzerPin = 10;         //pin that will drive the buzzer

// Only play a tone if the distance is in this range:
float NO_TONE_DISTANCE = 4.0;
float max_distance = 34.0;

/*
  note  frequency
  c     262 Hz
  d     294 Hz
  e     330 Hz
  f     349 Hz
  g     392 Hz
  a     440 Hz
  b     494 Hz
  C     523 Hz
*/

float FIRST_NOTE = 131.0;   // C3
float piutch_step = 2.0;    // inches per semitone


float map_to_pitch(float distance) {
  float steps = (distance - NO_TONE_DISTANCE) / piutch_step;
  float pitch = FIRST_NOTE * pow(1.05946, steps);
  // input to the tone function is unsigned int
  return (int)(floor(pitch));
}


////////////////////////////////////////////////////////////
// Setup and loop:

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  float distance = getDistance();
  if (distance < NO_TONE_DISTANCE || distance > max_distance) {
    Serial.print(distance);
    Serial.println(" inches -> no tone");
    noTone(buzzerPin);
  } else {
    float pitch = map_to_pitch(distance);
    Serial.print(distance);
    Serial.print(" inches -> ");
    Serial.println(pitch);
    tone(buzzerPin, pitch);
    delay(10);
  }
}

