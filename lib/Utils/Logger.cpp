#include "Logger.h"

void SerialLogger::logn(const char *label, const std::string &s)
{
    if (getEnabled())
        return;

    Serial.printf(getElapsedMilliseconds());
    Serial.printf(" [%s] ", label);
    Serial.println(s.c_str());
};