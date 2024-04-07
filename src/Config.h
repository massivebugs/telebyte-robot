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

    std::uint16_t getPCALeftShoulderPin()
    {
        return pcaLeftShoulderPin;
    }

    std::uint16_t getPCALeftElbowPin()
    {
        return pcaLeftElbowPin;
    }

    std::uint16_t getPCALeftWristPin()
    {
        return pcaLeftWristPin;
    }

    std::uint16_t getPCARightShoulderPin()
    {
        return pcaRightShoulderPin;
    }

    std::uint16_t getPCARightElbowPin()
    {
        return pcaRightElbowPin;
    }

    std::uint16_t getPCARightWristPin()
    {
        return pcaRightWristPin;
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
        doc["PCA_LEFT_SHOULDER_PIN"] = pcaLeftShoulderPin;
        doc["PCA_LEFT_ELBOW_PIN"] = pcaLeftElbowPin;
        doc["PCA_LEFT_WRIST_PIN"] = pcaLeftWristPin;
        doc["PCA_RIGHT_SHOULDER_PIN"] = pcaRightShoulderPin;
        doc["PCA_RIGHT_ELBOW_PIN"] = pcaRightElbowPin;
        doc["PCA_RIGHT_WRIST_PIN"] = pcaRightWristPin;
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
    std::uint16_t pcaLeftShoulderPin;
    std::uint16_t pcaLeftElbowPin;
    std::uint16_t pcaLeftWristPin;
    std::uint16_t pcaRightShoulderPin;
    std::uint16_t pcaRightElbowPin;
    std::uint16_t pcaRightWristPin;
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
        pcaLeftShoulderPin = pref.getUShort("pcaLeftShoulderPin", FPCA_LEFT_SHOULDER_PIN);
        pcaLeftElbowPin = pref.getUShort("pcaLeftElbowPin", FPCA_LEFT_ELBOW_PIN);
        pcaLeftWristPin = pref.getUShort("pcaLeftWristPin", FPCA_LEFT_WRIST_PIN);
        pcaRightShoulderPin = pref.getUShort("pcaRightShoulderPin", FPCA_RIGHT_SHOULDER_PIN);
        pcaRightElbowPin = pref.getUShort("pcaRightElbowPin", FPCA_RIGHT_ELBOW_PIN);
        pcaRightWristPin = pref.getUShort("pcaRightWristPin", FPCA_RIGHT_WRIST_PIN);
        sg90PWMMin = pref.getUShort("sg90PWMMin", FOSOYOO_SG90_PWM_MIN);
        sg90PWMMax = pref.getUShort("sg90PWMMax", FOSOYOO_SG90_PWM_MAX);
        wifiSSID = pref.getString("wifiSSID", FWIFI_SSID).c_str();
        wifiPwd = pref.getString("wifiPwd", FWIFI_PWD).c_str();
        wifiSSID_AP = pref.getString("wifiSSID_AP", FWIFI_SSID_AP).c_str();
        wifiPwd_AP = pref.getString("wifiPwd_AP", FWIFI_PWD_AP).c_str();
    }
};

#endif