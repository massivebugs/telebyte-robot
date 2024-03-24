#ifndef TELEBYTE_SYSTEMS
#define TELEBYTE_SYSTEMS

#include "all.h"
#include "Config.h"
#include "Timer.h"
#include "Events.h"
#include "Logger.h"
#include "WifiConnection.h"
#include "WebServer.h"
#include "CLI.h"

class Systems
{
public:
    void initialize(Config &config)
    {
        wifi = std::unique_ptr<WifiConnection>(new WifiConnection(config.getWifiSSID(), config.getWifiPwd()));
        webServer = std::unique_ptr<WebServer>(new WebServer(config.getMdnsHost(), config.getWebServerPort()));

        // Enable serial comms if we are in debug mode
        if (config.getDebug())
        {
            Serial.begin(config.getSerialBaud());
            while (!Serial)
            {
                ; // wait for serial port to connect. Needed for native USB
            }
            logger = std::unique_ptr<DebugLogger>(new DebugLogger(&timer));
        }
        else
        {
            logger = std::unique_ptr<ProductionLogger>(new ProductionLogger(&timer));
        }

        logger->logn("Systems", "All systems, Full Power!");
        timer.reset();

        logger->logn("Systems", "Connecting to Wi-Fi...");
        wifi->connect();

        logger->logn("Systems", "IP Address is: " + wifi->getLocalIP());

        logger->logn("Systems", "Starting API Server...");
        try
        {
            webServer->startMDNSResponder();
        }
        catch (WebServer::MDNSResponderSetupFailed &e)
        {
            logger->logn("Systems", e.what());
        }
        webServer->listen();

        logger->logn("Systems", "Setting PWM Driver...");
        pwmDriver.begin();
        pwmDriver.setPWMFreq(50);

        logger->logn("Systems", "Initialization finished!");
    }

public:
    Timer timer{};
    Adafruit_PWMServoDriver pwmDriver{};
    CLI cli{};

    std::unique_ptr<Logger> logger;
    std::unique_ptr<WifiConnection> wifi;
    std::unique_ptr<WebServer> webServer;

    struct SystemEvents
    {
        Event<void> helloWorld;
    } events{};
};

#endif