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
        setupAPI();
        systems.initialize();
        robot.initialize();
    }

    void execute(std::uint16_t delayMs = 500)
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
                              { request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"ping\":\"OK\"}"); });

        // Command
        systems.webServer.POST("/api/command", [&](AsyncWebServerRequest *request)
                               { 
                                String command;
                                if (request->hasArg("c")) {
                                    command = request->arg("c");
                                }                                 

                                auto result = systems.cli.parse(command.c_str());
                                handleCommand(result);

                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"status\":\"OK\"}"); });
    }

    static constexpr const char *COMMAND_HELLO_WORLD = "hello_world";
    void handleCommand(CLIInput &command)
    {
        // Register command handlers here
        if (command.name == COMMAND_HELLO_WORLD)
        {
            systems.events.helloWorld.publish(nullptr);
        }
    }

private:
    Systems systems{};
    Robot robot;
};

#endif