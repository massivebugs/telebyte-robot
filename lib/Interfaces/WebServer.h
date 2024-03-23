#ifndef INTERFACES_WEB_SERVER
#define INTERFACES_WEB_SERVER

#include <cstdint>
#include <functional>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>

class WebServer
{
public:
    class MDNSResponderSetupFailed : public std::exception
    {
    public:
        const char *what() const noexcept override
        {
            return "Error setting up MDNS responder";
        }
    };

public:
    WebServer(std::string host, const std::uint8_t port) : host{host}, server{port} {};

    void GET(const char *path, ArRequestHandlerFunction handler);

    void POST(const char *path, ArRequestHandlerFunction handler);

    void POST(const char *path, ArJsonRequestHandlerFunction handler);

    void listen();

    void startMDNSResponder();

public:
    static constexpr const char *CONTENT_TYPE_JSON = "application/json";

private:
    std::string host;
    AsyncWebServer server;
};

#endif