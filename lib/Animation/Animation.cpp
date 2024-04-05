#include "Animation.h"

void AnimationSequence::push(Animation animation)
{
    m_sequence.push(animation);
}

bool AnimationSequence::animate(long long currTime)
{
    // If the animation ends, let the caller know.
    if (m_sequence.empty())
    {
        return true;
    }

    if (m_sequence.front().animate(currTime))
    {
        m_sequence.pop();
    }
}
