#include "Controller.h"

void Controller::setup()
{
    config.initialize();
    systems.initialize(config);
    robot.initialize(&config, &systems);

    setupAPIEndpoints();
    setupBehaviors();
}

void Controller::execute(std::uint16_t delayMs = 100)
{
    if (config.getDebug() && Serial.available() > 0)
    {
        auto command = Serial.readString();
        auto result = systems.cli.parse(command.c_str());
        handleDebugCommand(result);
    }

    robot.executeRoutine(rootNode, &ctx);

    delay(delayMs);
}

void Controller::handleDebugCommand(CLIInput &command)
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