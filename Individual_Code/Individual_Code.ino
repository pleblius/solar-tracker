#include <Servo.h>

/*
 *            DEFINITIONS
 */

//Servo Pins
#define YAW_SIGNAL 9
#define PITCH_SIGNAL 11

//Photoresistor Pins
#define TL_PIN A2
#define TR_PIN A1
#define BL_PIN A3
#define BR_PIN A0

//LED Pins
#define LED_PIN 13


/*
 *            GLOBAL OBJECTS
 */

//Servo Block
Servo yawServo;
Servo pitchServo;

int rotAmount = 1;          //Max servo motion in one loop (degrees)
int delta = 80;             //Acceptable error between voltage readings

#define MAX_YAW 180
#define MIN_YAW 0
#define MAX_PITCH 80
#define MIN_PITCH 10
#define MID_YAW 90
#define MID_PITCH 45

/*
 *            Global Variables
 */

 int curYaw;
 int curPitch;

 int TR;
 int TL;
 int BR;
 int BL;
 
void setup() {
  yawServo.attach(YAW_SIGNAL);
  pitchServo.attach(PITCH_SIGNAL);

  pinMode(TL_PIN,INPUT);
  pinMode(TR_PIN,INPUT);
  pinMode(BL_PIN,INPUT);
  pinMode(BR_PIN,INPUT);

  pinMode(LED_PIN,OUTPUT);

  Serial.begin(9600);
  Serial.println("TL LDR\t\tTR LDR\t\t BL LDR\t\tBR LDR\t\tcurYaw\t\tcurPitch");

  yawServo.write(MID_YAW);
  pitchServo.write(MID_PITCH);
  curYaw = MID_YAW;
  curPitch = MID_PITCH;

  delay(5000);
}

void loop() {  
  TL = getReadings(TL_PIN);
  TR = getReadings(TR_PIN);
  BL = getReadings(BL_PIN);
  BR = getReadings(BR_PIN);

  if (!isDark()) {
    rotate();
  }

  printer();
  delay(500);
}

int yawDir() {
  int diffTop;
  int diffBot;
  int dir;

  diffTop = TR - TL;
  diffBot = BR - BL;

  if (abs(diffTop) > abs(diffBot)) {
    if (abs(diffTop) > delta) {
      dir = abs(diffTop)/diffTop;
    }
    else dir = 0;
  }
  else {
    if (abs(diffBot) > delta) {
      dir = abs(diffBot)/diffBot;
    }
    else dir = 0;
  }

  return dir;
}

int pitchDir() {
  int diffLeft;
  int diffRight;
  int dir;

  diffLeft = BL - TL;
  diffRight = BR - TR;

  if (abs(diffLeft) > abs(diffRight)) {
    if (abs(diffLeft) > delta) {
      dir = abs(diffLeft)/diffLeft;
    }
    else dir = 0;
  }
  else {
    if (abs(diffRight) > delta) {
      dir = abs(diffRight)/diffRight;
    }
    else dir = 0;
  }

  return dir;
}

void rotate() {
  int newYaw;
  int newPitch;

  newYaw = curYaw + yawDir();
  newPitch = curPitch + pitchDir();

  if (newYaw > MAX_YAW || newYaw < MIN_YAW) newYaw = curYaw;
  if (newPitch > MAX_PITCH || newPitch < MIN_PITCH) newPitch = curPitch;

  yawServo.write(newYaw);
  pitchServo.write(newPitch);

  curYaw = newYaw;
  curPitch = newPitch;
}

int getReadings(int pin) {
  int val = analogRead(pin);
  return val;
}

bool isDark() {
  if (TL < 100 && TR < 100 && BL < 100 && BR < 100) return true;
  else return false;
}

void printer() {
  Serial.print(TL);
  Serial.print("\t\t");
  Serial.print(TR);
  Serial.print("\t\t");
  Serial.print(BL);
  Serial.print("\t\t");
  Serial.print(BR);
  Serial.print("\t\t");
  Serial.print(curYaw);
  Serial.print("\t\t");
  Serial.print(curPitch);
  Serial.print("\n");
}
