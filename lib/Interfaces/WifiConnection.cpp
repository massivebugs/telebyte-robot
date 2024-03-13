#include "WifiConnection.h"

void WifiConnection::connect()
{
    WiFi.begin(ssid.c_str(), password.c_str());

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
};

std::string WifiConnection::getLocalIP()
{
    return std::string{WiFi.localIP().toString().c_str()};
};