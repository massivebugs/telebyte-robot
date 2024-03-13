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
    Systems() : logger{&timer},
                wifi{config.wifiSSID, config.wifiPWD},
                webServer{config.mdnsHost, config.webServerPort}
    {
    }

    void initialize()
    {
        // Enable serial comms if we are in debug mode
        if (config.debug)
        {
            Serial.begin(config.serialBaud);
            while (!Serial)
            {
                ; // wait for serial port to connect. Needed for native USB
            }
        }
        else
        {
            logger.disable();
        }

        logger.logn("Systems", "All systems, Full Power!");
        timer.reset();

        logger.logn("Systems", "Connecting to Wi-Fi...");
        wifi.connect();

        logger.logn("Systems", "IP Address is: " + wifi.getLocalIP());

        logger.logn("Systems", "Starting API Server...");
        try
        {
            webServer.startMDNSResponder();
        }
        catch (WebServer::MDNSResponderSetupFailed &e)
        {
            logger.logn("Systems", e.what());
        }
        webServer.listen();

        logger.logn("Systems", "Setting PWM Driver...");
        pwmDriver.begin();
        pwmDriver.setPWMFreq(50);

        logger.logn("Systems", "Initialization finished!");
    }

public:
    Config config{};
    Timer timer{};
    SerialLogger logger;
    WifiConnection wifi;
    WebServer webServer;
    Adafruit_PWMServoDriver pwmDriver{};
    CLI cli{};

    struct SystemEvents
    {
        Event<void> helloWorld;
    } events{};
};

#endif