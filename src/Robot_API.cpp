#include "Robot.h"

bool Robot::isPhoneSet()
{
    return fsrPhone->getValue() >= m_config->getFSRPhoneThreshold();
}