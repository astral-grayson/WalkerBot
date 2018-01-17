class Leg {

  /*
  Legs are defined in their own class, for maximum parallelism.
  They will always be called to move in the order they are
  defined so as to avoid unbalancing the robot
  Each one consists of 3 servos, addressed from top to bottom
  0 is all the way forward, 180 is all the way backwards (but
  generally avoid extreme values because the servos are less
  reliable towards their limits
  */
  Servo hip;
  Servo knee;
  Servo ankle;

  // settable limits, the servo will not move past these
  // use them to prevent the servo burning out trying to force
  // parts into each other
  int hipLowerLimit;
  int hipUpperLimit;
  int kneeLowerLimit;
  int kneeUpperLimit;
  int ankleLowerLimit;
  int ankleUpperLimit;

  // motion is achieved by setting a target,
  // then incrementing the current angle by moveSpeed degrees
  int hipTargetAngle;
  int kneeTargetAngle;
  int ankleTargetAngle;

  int moveSpeed;

  int hipCurrentAngle;
  int kneeCurrentAngle;
  int ankleCurrentAngle;

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


public:
  Leg(int hipPort, int kneePort, int anklePort, int hipLowerLimit=0, int hipUpperLimit=180, int kneeLowerLimit=0, int kneeUpperLimit=180, int ankleLowerLimit=0, int ankleUpperLimit=180, int moveSpeed=1) {
    // the constructor - note the many optional args
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

  void moveJoints() {
    // increment all joints
    this -> moveHip();
    this -> moveKnee();
    this -> moveAnkle();
  };

  void setTargets(int hipAngle, int kneeAngle, int ankleAngle) {
    // set target angles for all joints
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
    this -> setTargets(0,180,0);
  };

  void collapseInverse() {
    this -> setTargets(180,0,180);
  };

  void straighten() {
    this -> setTargets(90,90,90);
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
