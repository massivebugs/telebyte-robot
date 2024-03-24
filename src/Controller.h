#ifndef TELEBYTE_CONTROLLER
#define TELEBYTE_CONTROLLER

#include "all.h"
#include "Systems.h"
#include "Robot.h"

class Controller
{
public:
    void setup()
    {
        config.initialize();
        systems.initialize(config);
        robot.initialize(&config, &systems);
        setupAPI();
    }

    void execute(std::uint16_t delayMs = 100)
    {
        if (config.getDebug() && Serial.available() > 0)
        {
            auto command = Serial.readString();
            auto result = systems.cli.parse(command.c_str());
            handleCommand(result);
        }

        robot.executeRoutine();

        delay(delayMs);
    }

private:
    void setupAPI()
    {
        // Ping
        systems.webServer->GET("/api/ping", [&](AsyncWebServerRequest *request)
                               {
                                String message = "OK";
                                if (request->hasArg("msg")) {
                                    message = request->arg("msg");
                                }

                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"ping\":\"" + message + "\"}"); });

        // Command
        systems.webServer->POST("/api/command", [&](AsyncWebServerRequest *request, JsonVariant &json)
                                { 
                                std::string command = json["c"];
                                auto result = systems.cli.parse(command.c_str());

                                handleCommand(result);
                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"status\":\"OK\"}"); });

        // List config
        systems.webServer->GET("/api/config", [&](AsyncWebServerRequest *request)
                               { request->send(200, WebServer::CONTENT_TYPE_JSON, config.toJSON().c_str()); });
    }

    void handleCommand(CLIInput &command)
    {
        // Register command handlers here
        if (command.name == "ping")
        {
            systems.logger->logn("Controller", "ping");
            for (auto arg : command.args)
            {
                systems.logger->logn("Controller", "ping arg: " + arg);
            }
        }
        else if (command.name == "hello_world")
        {
            systems.events.helloWorld.publish(nullptr);
        }
    }

private:
    Config config{};
    Systems systems{};
    Robot robot{};
};

#endif