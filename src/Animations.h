#ifndef TELEBYTE_ANIMATIONS
#define TELEBYTE_ANIMATIONS

#include "all.h"
#include "Animation.h"
#include "Robot.h"
#include "Arm.h"

class AnimationRotateArm : public Animation
{
public:
    AnimationRotateArm(
        std::uint16_t totalDurationMs,
        std::shared_ptr<Arm> arm,
        Arm::Angles<std::int32_t> angles,
        std::uint8_t repeats = 0,
        bool alternate = false)
        : arm{arm},
          m_angles{angles},
          Animation{totalDurationMs, repeats, alternate} {}

    AnimationRotateArm(
        std::uint16_t totalDurationMs,
        std::shared_ptr<Arm> arm,
        Arm::Angles<std::int32_t> angles,
        bool loop,
        bool alternate = false)
        : arm{arm},
          m_angles{angles},
          Animation{totalDurationMs, loop, alternate} {}

private:
    std::shared_ptr<Arm> arm;
    Arm::Angles<std::int32_t> m_angles;
    Arm::Angles<std::int32_t> m_initialAngles;
    Arm::Angles<std::int32_t> m_anglesDiff;

    void begin() override
    {
        m_initialAngles = arm->getAngles<std::int32_t>();
        m_anglesDiff = m_angles - m_initialAngles;
    }

    void doAnimation(long long elapsedTimeMs) override
    {
        arm->rotate(
            m_initialAngles.shoulder + getAngle(m_anglesDiff.shoulder),
            m_initialAngles.elbow + getAngle(m_anglesDiff.elbow),
            m_initialAngles.wrist + getAngle(m_anglesDiff.wrist));
    }

    std::int32_t getAngle(std::int32_t angle)
    {
        return angle * getCurrentTimeRatio();
    }
};

#endif