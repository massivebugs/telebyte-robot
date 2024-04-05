#include "Controller.h"

void Controller::setupAPIEndpoints()
{
    // Ping
    systems.webServer->GET("/api/ping", [&](AsyncWebServerRequest *request)
                           {
                                String message = "OK";
                                if (request->hasArg("msg")) {
                                    message = request->arg("msg");
                                }

                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"pong\":\"" + message + "\"}"); });

    // Command
    systems.webServer->POST("/api/command", [&](AsyncWebServerRequest *request, JsonVariant &json)
                            { 
                                std::string command = json["c"];
                                auto result = systems.cli.parse(command.c_str());

                                handleDebugCommand(result);

                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"status\":\"OK\"}"); });

    // List config
    systems.webServer->GET("/api/config", [&](AsyncWebServerRequest *request)
                           { request->send(200, WebServer::CONTENT_TYPE_JSON, config.toJSON().c_str()); });
}
