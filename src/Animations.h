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
        Arm::Angles<std::int32_t> angles)
        : arm{arm},
          m_angles{angles},
          Animation{totalDurationMs} {}

private:
    std::shared_ptr<Arm> arm;
    Arm::Angles<std::int32_t> m_angles;
    Arm::Angles<std::int32_t> m_initialAngles;
    Arm::Angles<std::int32_t> m_anglesDiff;

    void doAnimation(long long elapsedTimeMs) override
    {
        if (elapsedTimeMs == 0)
        {
            m_initialAngles = arm->getAngles<std::int32_t>();
            m_anglesDiff = m_angles - m_initialAngles;
        }

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