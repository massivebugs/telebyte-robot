#include "Animation.h"

Animation::Animation(std::uint16_t durationMs, std::uint8_t repeats, bool alternate) : m_durationMs{durationMs},
                                                                                       m_repeats{repeats},
                                                                                       m_loop{false},
                                                                                       m_alternate{alternate}
{
}

Animation::Animation(std::uint16_t durationMs, bool loop, bool alternate) : m_durationMs{durationMs},
                                                                            m_repeats{0},
                                                                            m_loop{loop},
                                                                            m_alternate{alternate}
{
}

void Animation::begin() {}

void Animation::end() {}

bool Animation::animate(long long currTimeMs)
{
    if (isComplete())
    {
        end();
        return true;
    }
    else if (isDurationEnd())
    {
        resetForNewIteration(currTimeMs);
    }

    if (m_initialTimeMs == 0)
    {
        begin();
        m_initialTimeMs = currTimeMs;
    }

    m_elapsedTimeMs = currTimeMs - m_initialTimeMs;

    doAnimation(m_elapsedTimeMs);

    return false;
}

void Animation::reset()
{
    m_currRepeatCount = 0;
    m_initialTimeMs = 0;
    m_elapsedTimeMs = 0;
}

void Animation::resetForNewIteration(long long currTimeMs)
{
    m_currRepeatCount++;
    m_initialTimeMs = currTimeMs;
}

float Animation::getCurrentTimeRatio()
{
    auto ratio = std::min(
        1.0f,
        static_cast<float>(m_elapsedTimeMs) / static_cast<float>(m_durationMs));

    if (m_alternate)
    {
        ratio = ratio * 2;

        if (ratio > 1)
        {
            ratio = 2 - ratio;
        }
    }

    return ratio;
}

bool Animation::isComplete()
{
    // Complete if we aren't in a loop, iterations have ended and time is up.
    return !m_loop && isIterationsFinished() && isDurationEnd();
}

void Animation::setLoop(bool loop)
{
    m_loop = loop;
}

bool Animation::isDurationEnd()
{
    return m_durationMs <= m_elapsedTimeMs;
}

bool Animation::isIterationsFinished()
{
    return m_repeats == m_currRepeatCount;
}