/*

WalkerBot code for a small 3D printed walking robot
Written by Oliver Grayson, 2018


*/

#include <Servo.h>
#include <SoftwareSerial.h>
#include <Leg.h>

// globals
byte mode = 0; // 0:stopped, 1:creep, 2:?
byte timeScale = 0; // slow down/speed up robot
byte globalState = 0; // for all gait functions

SoftwareSerial Bluetooth(A0, A1); // bluetooth connection, can be replaced by USB
boolean bluetoothConnected = false;
String input = "";

Leg frontLeft(3,4,5);
Leg rearRight(12,13,14);
Leg frontRight(6,7,8);
Leg rearLeft(9,10,11);


/*
*  Movement utility functions
*/

void moveTo(Leg legA, int legAmoveH,int legAmoveK,int legAmoveA, Leg legB, int legBmoveH,int legBmoveK,int legBmoveA, Leg legC, int legCmoveH,int legCmoveK,int legCmoveA, Leg legD, int legDmoveH,int legDmoveK,int legDmoveA) {
  legA.setTargets(legAmoveH,legAmoveK,legAmoveA);
  legB.setTargets(legBmoveH,legBmoveK,legBmoveA);
  legC.setTargets(legCmoveH,legCmoveK,legCmoveA);
  legD.setTargets(legDmoveH,legDmoveK,legDmoveA);
  while (!legA.isReady() && !legB.isReady() && !legC.isReady() && legD.isReady()) {
    legA.moveJoints();legB.moveJoints();legC.moveJoints();legD.moveJoints();
    delay(timeScale);
  }
}

void moveToTarget(Leg l) {
  while (!l.isReady()) {
    l.moveJoints();
    delay(timeScale);
  }
}

void moveAllToTargets() {
  while (!frontRight.isReady() && !rearLeft.isReady() && !frontLeft.isReady() && rearRight.isReady()) {
    frontRight.moveJoints();rearLeft.moveJoints();frontLeft.moveJoints();rearRight.moveJoints();
    delay(timeScale);
  }
}

void stand() {
  // moveTo(frontLeft, 90,90,90, rearRight, 90,90,90, frontRight, 90,90,90, rearLeft, 90,90,90);
  frontLeft.straighten();rearRight.straighten();frontRight.straighten();rearLeft.straighten();
  moveAllToTargets();
}


/*
*  Gait functions
*/

void creepBeat(Leg lift, Leg front, Leg center, Leg rear) {
  // lift one leg while all others slowly move backwards
  // called continuously with order of legs changing

  // raise lifted leg, move center and rear legs backwards
  moveTo(lift, 0,160,70, front, 0,0,0, center, 0,0,0, rear, 120,90,60);

  // move lifted leg forwards, all other legs move backwards
  moveTo(lift, 0,30,60, front, 0,0,0, center, 0,0,0, rear, 120,90,75);

  // set lifted leg down, all other legs continue moving backwards
  moveTo(lift, 0,30,90, front, 0,0,0, center, 0,0,0, rear, 120,90,90);

  // cycle state for different leg order
  if (globalState < 3) {
    globalState++;
  }
  else {
    globalState = 0;
  }
}


/*
*  Arduino functions
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  if (!Serial) {
    Bluetooth.begin(9600);
    Bluetooth.println("_______WalkerBot Initialized_______");
    bluetoothConnected = true;
  }
  else {
    Serial.println("_______WalkerBot Initialized_______");
  }
  stand();

}

void loop() {

  if (bluetoothConnected) {
    if (Bluetooth.available()) {
    // get Bluetooth input
      input = Bluetooth.read();
    }
    else {
      input = "";
    }
  }
  else {
    if (Serial.available()) {
      input = Serial.read();
    }
    else {
      input = "";
    }
  }

  if (input == "creep") {
    if (mode != 1) {
      mode = 1;
      stand();
      globalState = 0;
    }
  }
  else if (input == "stop") {
    if (mode != 0) {
      stand();
      mode = 0;
      globalState = 0;
    }
  }
  else if (input == "reset") {
    globalState = 0;
  }
  else if (input == "fast") {
    frontLeft.setMoveSpeed(10);
    rearRight.setMoveSpeed(10);
    frontRight.setMoveSpeed(10);
    rearLeft.setMoveSpeed(10);
  }
  else if (input == "slow") {
    frontLeft.setMoveSpeed(1);
    rearRight.setMoveSpeed(1);
    frontRight.setMoveSpeed(1);
    rearLeft.setMoveSpeed(1);
  }


  switch(mode) {
    case 1:
      switch(globalState) {
        case 0:
          creepBeat(rearRight, frontLeft, rearLeft, frontRight);
          break;
        case 1:
          creepBeat(frontRight, rearRight, frontLeft, rearLeft);
          break;
        case 2:
          creepBeat(rearLeft, frontLeft, rearLeft, frontRight);
          break;
        case 3:
          creepBeat(frontLeft, rearLeft, frontRight, rearRight);
          break;
      }
  }
}
