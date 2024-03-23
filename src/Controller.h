#ifndef TELEBYTE_CONTROLLER
#define TELEBYTE_CONTROLLER

#include "all.h"
#include "Systems.h"
#include "Robot.h"

class Controller
{
public:
    Controller() : robot{&systems}
    {
    }

    void setup()
    {
        systems.initialize();
        robot.initialize();
        setupAPI();
    }

    void execute(std::uint16_t delayMs = 100)
    {
        // TODO: Commands can come from Serial and via Web API
        if (Serial && Serial.available() > 0)
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
        systems.webServer.GET("/api/ping", [&](AsyncWebServerRequest *request)
                              {
                                String message = "OK";
                                if (request->hasArg("msg")) {
                                    message = request->arg("msg");
                                }

                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"ping\":\"" + message + "\"}"); });

        // Command
        systems.webServer.POST("/api/command", [&](AsyncWebServerRequest *request, JsonVariant &json)
                               { 
                                std::string command = json["c"];
                                auto result = systems.cli.parse(command.c_str());

                                handleCommand(result);
                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"status\":\"OK\"}"); });

        // List config
        systems.webServer.GET("/api/config", [&](AsyncWebServerRequest *request)
                              { request->send(200, WebServer::CONTENT_TYPE_JSON, systems.config.toJSON().c_str()); });
    }

    static constexpr const char *COMMAND_PING{"ping"};
    static constexpr const char *COMMAND_HELLO_WORLD{"hello_world"};
    void handleCommand(CLIInput &command)
    {
        // Register command handlers here
        if (command.name == COMMAND_PING)
        {
            systems.logger->logn("Controller", "ping");
            for (auto arg : command.args)
            {
                systems.logger->logn("Controller", "ping arg: " + arg);
            }
        }
        else if (command.name == COMMAND_HELLO_WORLD)
        {
            systems.events.helloWorld.publish(nullptr);
        }
    }

private:
    Systems systems{};
    Robot robot;
};

#endif