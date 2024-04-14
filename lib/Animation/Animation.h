#ifndef ANIMATION_ANIMATION
#define ANIMATION_ANIMATION

#include <cstdint>
#include <string>
#include <queue>

class Animation
{
public:
    Animation(std::uint16_t durationMs, std::uint8_t repeats = 0, bool alternate = false);
    Animation(std::uint16_t durationMs, bool loop = false, bool alternate = false);

    bool animate(long long currTimeMs);

    void reset();

    float getCurrentTimeRatio();

    bool isComplete();

    void setLoop(bool loop);

private:
    virtual void begin();
    virtual void end();
    virtual void doAnimation(long long elapsedTimeMs) = 0;

    void resetForNewIteration(long long currTimeMs);
    bool isDurationEnd();
    bool isIterationsFinished();

    std::uint16_t m_durationMs;
    std::uint8_t m_repeats;
    bool m_loop;
    bool m_alternate;

    std::uint8_t m_currRepeatCount{0};
    long long m_initialTimeMs{0};
    long long m_elapsedTimeMs{0};
};

#endif