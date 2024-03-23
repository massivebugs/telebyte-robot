#ifndef UTILS_LOGGER
#define UTILS_LOGGER

#include <string>
#include <Arduino.h>
#include "Timer.h"

/**
 * @file Logger.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief Logger interface
 */

/**
 * @brief Provides an interface for logging messages.
 *
 * This class serves as a base logger with methods for logging messages with or without timestamps.
 */
class Logger
{
public:
    /**
     * @brief Constructs a new Logger object with a specified timer.
     *
     * @param timer A pointer to the Timer object used for timestamping log messages.
     */
    Logger(Timer *timer) : timer{timer} {}

    /**
     * @brief Logs a message with a label.
     *
     * @param label The label associated with the log message.
     * @param s The message to log.
     */
    virtual void logn(const char *label, const std::string &s) = 0;

    /**
     * @brief Destroys the Logger object.
     *
     * Virtual destructor to allow proper cleanup of derived classes.
     */
    virtual ~Logger() {}

    /**
     * @brief Checks if logging is enabled.
     *
     * @return bool
     */
    bool getEnabled() const
    {
        return enabled;
    }

    /**
     * @brief Disables logging.
     */
    void disable()
    {
        enabled = false;
    }

    /**
     * @brief Enables logging.
     */
    void enable()
    {
        enabled = true;
    }

    /**
     * @brief Gets the elapsed time in milliseconds as a string.
     *
     * @return const char*.
     */
    const char *getElapsedMilliseconds()
    {
        return std::to_string(timer->getElapsedMilliseconds()).c_str();
    }

private:
    Logger() = default;
    bool enabled{true};
    Timer *timer;
};

/**
 * @brief Provides functionality for logging messages via WebSockets.
 */
class ProductionLogger : public Logger
{
public:
    ProductionLogger(Timer *timer) : Logger(timer) {}

    void logn(const char *label, const std::string &s) override;

private:
    ProductionLogger() = default;
};

/**
 * @brief Provides functionality for logging messages to the serial port.
 */
class DebugLogger : public Logger
{
public:
    DebugLogger(Timer *timer) : Logger(timer) {}

    void logn(const char *label, const std::string &s) override;

private:
    DebugLogger() = default;
};

#endif