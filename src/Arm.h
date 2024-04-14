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

    template <typename T>
    struct Angles
    {
        T shoulder;
        T elbow;
        T wrist;

        template <typename std::enable_if<std::is_same<T, std::int32_t>::value, int>::type = 0>
        Angles operator-(const Angles<T> &other) const
        {
            return Angles<T>{
                shoulder - other.shoulder,
                elbow - other.elbow,
                wrist - other.wrist};
        }
    };

    Arm(
        Side ap,
        Servo<Adafruit_PWMServoDriver> shoulder,
        Servo<Adafruit_PWMServoDriver> elbow,
        Servo<Adafruit_PWMServoDriver> wrist) : ap{ap},
                                                shoulder{shoulder},
                                                elbow{elbow},
                                                wrist{wrist}
    {
    }

    void rotate(std::uint16_t shoulderAngle, std::uint16_t elbowAngle, std::uint16_t wristAngle)
    {
        shoulder.rotate(shoulderAngle);
        elbow.rotate(elbowAngle);
        wrist.rotate(wristAngle);
    }

    void rotateShoulder(std::uint16_t angle)
    {
        shoulder.rotate(angle);
    }

    void rotateElbow(std::uint16_t angle)
    {
        elbow.rotate(angle);
    }

    void rotateWrist(std::uint16_t angle)
    {
        wrist.rotate(angle);
    }

    template <typename T>
    Angles<T> getAngles()
    {
        return Angles<T>{
            shoulder.getAngle(),
            elbow.getAngle(),
            wrist.getAngle(),
        };
    }

private:
    Side ap;
    Servo<Adafruit_PWMServoDriver> shoulder;
    Servo<Adafruit_PWMServoDriver> elbow;
    Servo<Adafruit_PWMServoDriver> wrist;
};

#endif