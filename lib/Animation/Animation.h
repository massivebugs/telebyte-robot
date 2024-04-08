#ifndef ANIMATION_ANIMATION
#define ANIMATION_ANIMATION

#include <cstdint>
#include <string>
#include <queue>

class Animation
{
public:
    Animation(std::uint16_t totalDurationMs);

    bool animate(long long currTimeMs);

    void reset();

    float getCurrentTimeRatio();

private:
    friend class AnimationSequence;

    virtual void doAnimation(long long elapsedTimeMs) = 0;

    std::uint16_t m_totalDurationMs;
    long long m_initialTimeMs{0};
    long long m_elapsedTimeMs{0};
};

// class AnimationSequence
// {
// public:
//     void push(Animation animation);

//     bool animate(long long currTimeMs);

// private:
//     std::queue<Animation> m_sequence{};
// };

#endif