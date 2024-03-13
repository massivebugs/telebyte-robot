#ifndef INTERFACES_SERVO
#define INTERFACES_SERVO

#include <cstdint>

constexpr std::uint16_t SERVO_ANGLE_MIN = 0;
constexpr std::uint16_t SERVO_ANGLE_MAX = 180;

/**
 * @file Servo.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief Interface for a servo motor.
 *
 * This class provides functionality to control a servo motor connected to a PWM driver.
 *
 * @tparam T The type of the PWM driver.
 */
template <typename T>
class Servo
{
public:
    Servo(
        T *pwmDriver,
        std::uint16_t pinNum,
        std::uint16_t pwmMin,
        std::uint16_t pwmMax,
        std::uint16_t angleMin = SERVO_ANGLE_MIN,
        std::uint16_t angleMax = SERVO_ANGLE_MAX) : pwmDriver{pwmDriver},
                                                    pinNum{pinNum},
                                                    pwmMin{pwmMin},
                                                    pwmMax{pwmMax},
                                                    angleMin{angleMin},
                                                    angleMax{angleMax}

    {
    }

    std::uint16_t getAngle()
    {
        return angle;
    }

    void rotate(std::uint16_t newAngle)
    {
        angle = adjustAngleMinMax(newAngle);
        int pulse = map(angle, 0, 180, pwmMin, pwmMax);
        pwmDriver->setPWM(pinNum, 0, pulse);
    }

    void debugPrint()
    {
        Serial.printf("Servo(%d): %d\n", pinNum, angle);
    }

private:
    T *pwmDriver;
    std::uint16_t pwmMin;
    std::uint16_t pwmMax;
    std::uint16_t pinNum;
    std::uint16_t angleMin;
    std::uint16_t angleMax;
    std::uint16_t angle = 0;

    std::uint16_t adjustAngleMinMax(std::uint16_t angle)
    {
        if (angle < angleMin)
        {
            return angleMin;
        }

        if (angle > angleMax)
        {
            return angleMax;
        }

        return angle;
    }
};

#endif