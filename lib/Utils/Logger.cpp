#include "Logger.h"

void ProductionLogger::logn(const char *label, const std::string &s)
{
    if (!getEnabled())
        return;

    // TODO: Log via WebSockets
};

void DebugLogger::logn(const char *label, const std::string &s)
{
    if (!getEnabled())
        return;

    Serial.printf(getElapsedMilliseconds());
    Serial.printf(" [%s] ", label);
    Serial.println(s.c_str());

    // TODO: Log via WebSockets
};