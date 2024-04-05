#ifndef ANIMATION_ANIMATION
#define ANIMATION_ANIMATION

#include <cstdint>
#include <string>
#include <queue>

class Animation
{
public:
    Animation(std::uint16_t timeMs) : timeMs{timeMs}
    {
    }

    virtual bool animate(long long currTime);

private:
    friend AnimationSequence;

    std::uint16_t timeMs;
    long long elapsedTimeMs{0};
};

class AnimationSequence
{
public:
    void push(Animation animation);

    bool animate(long long currTime);

private:
    std::queue<Animation> m_sequence{};
};

#endif