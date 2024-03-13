#include "Timer.h"

void Timer::reset()
{
    initTime = time_point{};
};

long long Timer::getElapsedMilliseconds()
{
    auto currTime = clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(currTime - initTime).count();
};

long long Timer::getElapsedSeconds()
{
    return getElapsedMilliseconds() / 1000;
};