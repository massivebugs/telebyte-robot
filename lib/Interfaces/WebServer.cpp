#include "WebServer.h"

void WebServer::GET(const char *path, ArRequestHandlerFunction handler)
{
    server.on(path, HTTP_GET, handler);
}

void WebServer::POST(const char *path, ArRequestHandlerFunction handler)
{
    server.on(path, HTTP_POST, handler);
}

void WebServer::POST(const char *path, ArJsonRequestHandlerFunction handler)
{
    server.addHandler(new AsyncCallbackJsonWebHandler(path, handler));
}

void WebServer::listen()
{
    server.begin();
}

void WebServer::startMDNSResponder()
{
    if (!MDNS.begin(host.c_str()))
    {
        throw MDNSResponderSetupFailed{};
    }
}