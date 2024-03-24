#ifndef TELEBYTE_CONFIG
#define TELEBYTE_CONFIG

#include "all.h"

class Config
{
public:
    Config()
    {
    }

    void initialize()
    {
        pref.begin(NAMESPACE, true);
        setValues();
    }

    void resetToFactoryDefaults()
    {
        pref.clear();
        setValues();
    }

    bool getDebug()
    {
        return debug;
    }

    std::uint32_t getSerialBaud()
    {
        return serialBaud;
    }

    std::uint8_t getWebServerPort()
    {
        return webServerPort;
    }

    std::string getMdnsHost()
    {
        return mdnsHost;
    }

    std::uint16_t getFSRPhonePin()
    {
        return fsrPhone;
    }

    std::uint16_t getFSRPhoneThreshold()
    {
        return fsrPhoneThreshold;
    }

    std::uint16_t getPCAUL1Pin()
    {
        return pcaUL1;
    }

    std::uint16_t getPCAUL2Pin()
    {
        return pcaUL2;
    }

    std::uint16_t getPCAUL3Pin()
    {
        return pcaUL3;
    }

    std::uint16_t getPCAUR1Pin()
    {
        return pcaUR1;
    }

    std::uint16_t getPCAUR2Pin()
    {
        return pcaUR2;
    }

    std::uint16_t getPCAUR3Pin()
    {
        return pcaUR3;
    }

    std::uint16_t getSG90PWMMin()
    {
        return sg90PWMMin;
    }

    std::uint16_t getSG90PWMMax()
    {
        return sg90PWMMax;
    }

    std::string getWifiSSID()
    {
        return wifiSSID;
    }

    std::string getWifiPwd()
    {
        return wifiPwd;
    }

    std::string getWifiAPSSID()
    {
        return wifiSSID_AP;
    }

    std::string getWifiAPPwd()
    {
        return wifiPwd_AP;
    }

    std::string toJSON()
    {
        JsonDocument doc;

        doc["DEBUG"] = debug;
        doc["SERIAL_BAUD"] = serialBaud;
        doc["WEB_SERVER_PORT"] = webServerPort;
        doc["MDNS_HOST"] = mdnsHost;
        doc["FSR_PHONE"] = fsrPhone;
        doc["FSR_PHONE_THRESHOLD"] = fsrPhoneThreshold;
        doc["PCA_UL1"] = pcaUL1;
        doc["PCA_UL2"] = pcaUL2;
        doc["PCA_UL3"] = pcaUL3;
        doc["PCA_UR1"] = pcaUR1;
        doc["PCA_UR2"] = pcaUR2;
        doc["PCA_UR3"] = pcaUR3;
        doc["OSOYOO_SG90_PWM_MIN"] = sg90PWMMin;
        doc["OSOYOO_SG90_PWM_MAX"] = sg90PWMMax;
        doc["WIFI_SSID"] = wifiSSID;
        doc["WIFI_PWD"] = wifiPwd;
        doc["WIFI_SSID_AP"] = wifiSSID_AP;
        doc["WIFI_PWD_AP"] = wifiPwd_AP;

        std::string output;
        serializeJson(doc, output);

        return output;
    }

private:
    static constexpr const char *NAMESPACE{"config"};
    Preferences pref;

    bool debug;
    std::uint32_t serialBaud;
    std::uint8_t webServerPort;
    std::string mdnsHost;
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
    std::string wifiSSID;
    std::string wifiPwd;
    std::string wifiSSID_AP;
    std::string wifiPwd_AP;

    void setValues()
    {
        debug = pref.getBool("debug", FDEBUG);
        serialBaud = pref.getUInt("serialBaud", FSERIAL_BAUD);
        webServerPort = pref.getUShort("webServerPort", FWEB_SERVER_PORT);
        mdnsHost = pref.getString("mdnsHost", FMDNS_HOST).c_str();
        fsrPhone = pref.getUShort("fsrPhone", FFSR_PHONE);
        fsrPhoneThreshold = pref.getUShort("fsrPhoneThreshold", FFSR_PHONE_THRESHOLD);
        pcaUL1 = pref.getUShort("pcaUL1", FPCA_UL1);
        pcaUL2 = pref.getUShort("pcaUL2", FPCA_UL2);
        pcaUL3 = pref.getUShort("pcaUL3", FPCA_UL3);
        pcaUR1 = pref.getUShort("pcaUR1", FPCA_UR1);
        pcaUR2 = pref.getUShort("pcaUR2", FPCA_UR2);
        pcaUR3 = pref.getUShort("pcaUR3", FPCA_UR3);
        sg90PWMMin = pref.getUShort("sg90PWMMin", FOSOYOO_SG90_PWM_MIN);
        sg90PWMMax = pref.getUShort("sg90PWMMax", FOSOYOO_SG90_PWM_MAX);
        wifiSSID = pref.getString("wifiSSID", FWIFI_SSID).c_str();
        wifiPwd = pref.getString("wifiPwd", FWIFI_PWD).c_str();
        wifiSSID_AP = pref.getString("wifiSSID_AP", FWIFI_SSID_AP).c_str();
        wifiPwd_AP = pref.getString("wifiPwd_AP", FWIFI_PWD_AP).c_str();
    }
};

#endif