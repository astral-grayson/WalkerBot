#include <Servo.h>

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
    hip = Servo(hipPort);
    knee = Servo(kneePort);
    ankle = Servo(anklePort);

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
  }

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
  }

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
  }

  void move() {
    this.moveHip();
    this.moveKnee();
    this.moveAnkle();
  }

  void turnTo(int hipAngle, int kneeAngle, int ankleAngle) {
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

  Boolean isReady() {
    if (hipCurrentAngle == hipTargetAngle) {
      if (kneeCurrentAngle == kneeTargetAngle) {
        if (ankleCurrentAngle == ankleTargetAngle) {
          return true;
        }
      }
    }

    return false;
  }

  void collapse() {
    this.move(0,180,0);
  };

  void collapseInverse() {
    this.move(180,0,180);
  };

  void straighten() {
    this.move(90,90,90);
  };

  void setMoveSpeed(int speed) {
    moveSpeed = speed;
  }

  void setHipPort(int port) {
    hip = Servo(port);
  };

  void setKneePort(int port) {
    knee = Servo(port);
  };

  void setAnklePort(int port) {
    ankle = Servo(port);
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
