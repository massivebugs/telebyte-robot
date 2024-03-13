#ifndef INTERFACES_WIFI_CONNECTION
#define INTERFACES_WIFI_CONNECTION

#include <string>
#include <stdexcept>
#include <WiFi.h>

/**
 * @file WifiConnection.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief Represents a Wi-Fi connection manager.
 *
 * This class provides functionality to establish a Wi-Fi connection
 * with the specified network credentials and host name.
 */
class WifiConnection
{
public:
    /**
     * @brief Constructs a new WifiConnection object.
     *
     * @param ssid The SSID of the Wi-Fi network.
     * @param password The password of the Wi-Fi network.
     */
    WifiConnection(std::string ssid, std::string password) : ssid{ssid},
                                                             password{password} {};

    /**
     * @brief Connect to the Wi-Fi network.
     *
     */
    void connect();

    /**
     * @brief Get the local IP address
     */
    std::string getLocalIP();

private:
    std::string ssid;
    std::string password;
};

#endif