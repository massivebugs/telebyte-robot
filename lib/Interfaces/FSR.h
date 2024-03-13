#ifndef INTERFACES_FSR
#define INTERFACES_FSR

#include <cstdint>
#include <Arduino.h>

/**
 * @file FSR.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief Represents a Force Sensitive Resistor (FSR) sensor.
 *
 * This class provides functionality to interface with a Force Sensitive Resistor
 * sensor connected to a microcontroller.
 */
class FSR
{
public:
    /**
     * @brief Constructs a new FSR object with the specified pin number.
     *
     * @param pinNum The pin number to which the FSR sensor is connected.
     */
    FSR(std::uint16_t pinNum) : pinNum{pinNum} {};

    /**
     * @brief Reads the analog value from the FSR sensor and returns the value.

     * @return std::uint16_t value.
     */
    std::uint16_t read();

    /**
     * @brief Gets the last read analog value from the FSR sensor.
     *
     * @return std::uint16_t value.
     */
    std::uint16_t getValue();

private:
    std::uint16_t pinNum;
    // The last read analog value.
    std::uint16_t value = 0;
};

#endif