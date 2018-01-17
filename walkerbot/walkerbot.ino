#include <Servo.h>
#include <SoftwareSerial.h>
//#include <Leg.h>


class Leg {
  Servo hip;
  Servo knee;
  Servo ankle;

  int hipLowerLimit;
  int hipUpperLimit;
  int kneeLowerLimit;
  int kneeUpperLimit;
  int ankleLowerLimit;
  int ankleUpperLimit;

  int hipCurrentAngle;
  int kneeCurrentAngle;
  int ankleCurrentAngle;

  int hipTargetAngle;
  int kneeTargetAngle;
  int ankleTargetAngle;

  int moveSpeed;

public:
  Leg(int hipPort, int kneePort, int anklePort, int hipLowerLimit=0, int hipUpperLimit=180, int kneeLowerLimit=0, int kneeUpperLimit=180, int ankleLowerLimit=0, int ankleUpperLimit=180, int moveSpeed=1) {
    // constructor
    hip.attach(hipPort);
    knee.attach(kneePort);
    ankle.attach(anklePort);

    hipCurrentAngle = hip.read();
    kneeCurrentAngle = knee.read();
    ankleCurrentAngle = ankle.read();

    hipTargetAngle = hipCurrentAngle;
    kneeTargetAngle = kneeCurrentAngle;
    ankleTargetAngle = ankleCurrentAngle;

  };

  void moveHip() {
    if (hipCurrentAngle < hipTargetAngle) {
      hipCurrentAngle += moveSpeed;
      if (hipCurrentAngle < hipTargetAngle) {
        hip.write(hipCurrentAngle);
      }
      else {
        hip.write(hipTargetAngle);
        hipCurrentAngle = hipTargetAngle;
      }
    }
    if (hipCurrentAngle > hipTargetAngle) {
      hipCurrentAngle -= moveSpeed;
      if (hipCurrentAngle > hipTargetAngle) {
        hip.write(hipCurrentAngle);
      }
      else {
        hip.write(hipTargetAngle);
        hipCurrentAngle = hipTargetAngle;
      }
    }
  };

  void moveKnee() {
    if (kneeCurrentAngle < kneeTargetAngle) {
      kneeCurrentAngle += moveSpeed;
      if (kneeCurrentAngle < kneeTargetAngle) {
        knee.write(kneeCurrentAngle);
      }
      else {
        knee.write(kneeTargetAngle);
        kneeCurrentAngle = kneeTargetAngle;
      }
    }
    if (kneeCurrentAngle > kneeTargetAngle) {
      kneeCurrentAngle -= moveSpeed;
      if (kneeCurrentAngle > kneeTargetAngle) {
        knee.write(kneeCurrentAngle);
      }
      else {
        knee.write(kneeTargetAngle);
        kneeCurrentAngle = kneeTargetAngle;
      }
    }
  };

  void moveAnkle() {
    if (ankleCurrentAngle < ankleTargetAngle) {
      ankleCurrentAngle += moveSpeed;
      if (ankleCurrentAngle < ankleTargetAngle) {
        ankle.write(ankleCurrentAngle);
      }
      else {
        ankle.write(ankleTargetAngle);
        ankleCurrentAngle = ankleTargetAngle;
      }
    }
    if (ankleCurrentAngle > ankleTargetAngle) {
      ankleCurrentAngle -= moveSpeed;
      if (ankleCurrentAngle > ankleTargetAngle) {
        ankle.write(ankleCurrentAngle);
      }
      else {
        ankle.write(ankleTargetAngle);
        ankleCurrentAngle = ankleTargetAngle;
      }
    }
  };

  void moveJoints() {
    moveHip();
    moveKnee();
    moveAnkle();
  };

  void setTarget(int hipAngle, int kneeAngle, int ankleAngle) {
    if (hipAngle >= hipLowerLimit) {
      if (hipAngle <= hipUpperLimit) {
        hipTargetAngle = hipAngle;
      }
      else {
        hipTargetAngle = hipUpperLimit;
      }
    }
    else {
      hipTargetAngle = hipLowerLimit;
    }

    if (kneeAngle >= kneeLowerLimit) {
      if (kneeAngle <= kneeUpperLimit) {
        kneeTargetAngle = kneeAngle;
      }
      else {
        kneeTargetAngle = kneeUpperLimit;
      }
    }
    else {
      kneeTargetAngle = kneeLowerLimit;
    }

    if (ankleAngle >= ankleLowerLimit) {
      if (ankleAngle <= ankleUpperLimit) {
        ankleTargetAngle = ankleAngle;
      }
      else {
        ankleTargetAngle = ankleUpperLimit;
      }
    }
    else {
      ankleTargetAngle = ankleLowerLimit;
    }

  };

  boolean isReady() {
    if (hipCurrentAngle == hipTargetAngle) {
      if (kneeCurrentAngle == kneeTargetAngle) {
        if (ankleCurrentAngle == ankleTargetAngle) {
          return true;
        }
      }
    }

    return false;
  };

  void collapse() {
    this -> setTarget(0,180,0);
  };

  void collapseInverse() {
    this -> setTarget(180,0,180);
  };

  void straighten() {
    this -> setTarget(90,90,90);
  };

  void setMoveSpeed(int spd) {
    moveSpeed = spd;
  };

  void setHipPort(int port) {
    hip.attach(port);
  };

  void setKneePort(int port) {
    knee.attach(port);
  };

  void setAnklePort(int port) {
    ankle.attach(port);
  };

  void setHipLowerLimit(int limit) {
    hipLowerLimit = limit;
  };

  void setHipUpperLimit(int limit) {
    hipUpperLimit = limit;
  };

  void setKneeLowerLimit(int limit) {
    kneeLowerLimit = limit;
  };

  void setKneeUpperLimit(int limit) {
    kneeUpperLimit = limit;
  };

  void setAnkleLowerLimit(int limit) {
    ankleLowerLimit = limit;
  };

  void setAnkleUpperLimit(int limit) {
    ankleUpperLimit = limit;
  };

};


// globals
int mode = 0;
int timeScale = 0; // slow down/speed up robot
int globalState = 0; // for all gait functions

SoftwareSerial Bluetooth(A0, A1); // bluetooth connection, can be replaced by USB
boolean bluetoothConnected = false;
String input = "";

// Legs are defined in their own class, for maximum parallelism.
// They will always be called to move in the order they are
// defined so as to avoid unbalancing the robot
// Each one consists of 3 servos, addressed from top to bottom
// 0 is all the way forward, 180 is all the way backwards (but
// generally avoid extreme values because the servos are less
// reliable towards their limits
Leg frontLeft(3,4,5);
Leg rearRight(12,13,14);
Leg frontRight(6,7,8);
Leg rearLeft(9,10,11);


// user defined functions

void moveTo(Leg legA, int legAmoveH,int legAmoveK,int legAmoveA, Leg legB, int legBmoveH,int legBmoveK,int legBmoveA, Leg legC, int legCmoveH,int legCmoveK,int legCmoveA, Leg legD, int legDmoveH,int legDmoveK,int legDmoveA) {
  legA.setTarget(legAmoveH,legAmoveK,legAmoveA);
  legB.setTarget(legBmoveH,legBmoveK,legBmoveA);
  legC.setTarget(legCmoveH,legCmoveK,legCmoveA);
  legD.setTarget(legDmoveH,legDmoveK,legDmoveA);
  while (!legA.isReady() && !legB.isReady() && !legC.isReady() && legD.isReady()) {
    legA.moveJoints();legB.moveJoints();legC.moveJoints();legD.moveJoints();
    delay(timeScale);
  }
}

void stand() {
  moveTo(frontLeft, 90,90,90, rearRight, 90,90,90, frontRight, 90,90,90, rearLeft, 90,90,90);
}

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
