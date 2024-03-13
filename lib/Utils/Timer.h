#ifndef UTILS_SIMPLE_TIMER
#define UTILS_SIMPLE_TIMER

#include <chrono>

// struct TimerHandler
// {
//     long long lastTime;
//     const long long durationMs;
//     std::function<void(void)> func;
// };
// using Handler = std::function<void(void)>;

/**
 * @file Timer.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief Represents a Timer utility for measuring elapsed time.
 *
 * This class provides functionality to measure elapsed time since initialization
 * in milliseconds and seconds.
 * This does not utilize NTP to obtain time.
 */
class Timer
{
public:
    /**
     * @brief Reset the timer to 0
     */
    void reset();

    /**
     * @brief Gets the elapsed time since initialization in milliseconds.
     *
     * @return long long
     */
    long long getElapsedMilliseconds();

    /**
     * @brief Gets the elapsed time since initialization in seconds.
     *
     * @return long long
     */
    long long getElapsedSeconds();

private:
    // void runEvery(Duration durationMs, Handler handler)
    // {
    //     handlers.push_back({0, durationMs, handler});
    // }

private:
    using clock = std::chrono::steady_clock;
    using time_point = std::chrono::time_point<clock>;

    time_point initTime{};

    // std::vector<std::tuple<Duration, DurationMs, Handler>> handlers;
};

#endif