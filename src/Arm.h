#ifndef TELEBYTE_ARM
#define TELEBYTE_ARM

#include "all.h"
#include "Servo.h"

class Arm
{
public:
    enum class Side
    {
        LEFT,
        RIGHT
    };

    Arm(
        Side ap,
        Servo<Adafruit_PWMServoDriver> *shoulder,
        Servo<Adafruit_PWMServoDriver> *elbow,
        Servo<Adafruit_PWMServoDriver> *wrist) : ap{ap},
                                                 shoulder{shoulder},
                                                 elbow{elbow},
                                                 wrist{wrist}
    {
    }

    void rotate(uint16_t shoulderAngle, uint16_t elbowAngle, uint16_t wristAngle)
    {
        shoulder->rotate(shoulderAngle);
        elbow->rotate(elbowAngle);
        wrist->rotate(wristAngle);
    }

private:
    Side ap;
    Servo<Adafruit_PWMServoDriver> *shoulder;
    Servo<Adafruit_PWMServoDriver> *elbow;
    Servo<Adafruit_PWMServoDriver> *wrist;
};

#endif