#include "Animation.h"

Animation::Animation(std::uint16_t totalDurationMs) : m_totalDurationMs{totalDurationMs} {}

bool Animation::animate(long long currTimeMs)
{
    if (m_initialTimeMs == 0)
    {
        m_initialTimeMs = currTimeMs;
    }
    else
    {
        m_elapsedTimeMs = currTimeMs - m_initialTimeMs;
    }

    doAnimation(m_elapsedTimeMs);

    return m_totalDurationMs <= m_elapsedTimeMs;
}

void Animation::reset()
{
    m_initialTimeMs = 0;
    m_elapsedTimeMs = 0;
}

float Animation::getCurrentTimeRatio()
{
    return std::min(1.0f, static_cast<float>(m_elapsedTimeMs) / static_cast<float>(m_totalDurationMs));
}

// void AnimationSequence::push(Animation animation)
// {
//     m_sequence.push(animation);
// }

// bool AnimationSequence::animate(long long currTimeMs)
// {
//     // If the animation ends, let the caller know.
//     if (m_sequence.empty())
//     {
//         return true;
//     }

//     if (m_sequence.front().animate(currTimeMs))
//     {
//         m_sequence.pop();
//     }

//     return false;
// }
