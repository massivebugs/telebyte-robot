#include "FSR.h"

std::uint16_t FSR::read()
{
    value = analogRead(pinNum);
    return value;
};

std::uint16_t FSR::getValue()
{
    return value;
};