#ifndef TELEBYTE_CONFIG
#define TELEBYTE_CONFIG

#include "all.h"

class Config
{
public:
    Config()
    {
        // #ifdef FFACTORY_RESET_ON_BOOT
        debug = FDEBUG;
        serialBaud = FSERIAL_BAUD;
        webServerPort = FWEB_SERVER_PORT;
        mdnsHost = FMDNS_HOST;

        fsrPhone = FFSR_PHONE;
        fsrPhoneThreshold = FFSR_PHONE_THRESHOLD;
        pcaUL1 = FPCA_UL1;
        pcaUL2 = FPCA_UL2;
        pcaUL3 = FPCA_UL3;
        pcaUR1 = FPCA_UR1;
        pcaUR2 = FPCA_UR2;
        pcaUR3 = FPCA_UR3;

        sg90PWMMin = FOSOYOO_SG90_PWM_MIN;
        sg90PWMMax = FOSOYOO_SG90_PWM_MAX;

        wifiSSID = FWIFI_SSID;
        wifiPWD = FWIFI_PWD;

        wifiSSID_AP = FWIFI_SSID_AP;
        wifiPWD_AP = FWIFI_PWD_AP;
        // #endif
    }

    // Basic options
    bool debug;
    std::uint32_t serialBaud;
    std::uint8_t webServerPort;
    std::string mdnsHost;

    // Sensors etc
    std::uint16_t fsrPhone;
    std::uint16_t fsrPhoneThreshold;
    std::uint16_t pcaUL1;
    std::uint16_t pcaUL2;
    std::uint16_t pcaUL3;
    std::uint16_t pcaUR1;
    std::uint16_t pcaUR2;
    std::uint16_t pcaUR3;

    std::uint16_t sg90PWMMin;
    std::uint16_t sg90PWMMax;

    // Secrets
    std::string wifiSSID;
    std::string wifiPWD;

    std::string wifiSSID_AP;
    std::string wifiPWD_AP;
};
#endif