#pragma once
#include "../../Core/module_system/IModule.h"
#include <crow.h>
#include <thread>
#include <iostream>

namespace CentralPlatform::Modules::ApiGateway
{
    class HttpServerModule : public CentralPlatform::Core::IModule
    {
    private:
        crow::SimpleApp app;
        std::thread serverThread;

    public:
        HttpServerModule() = default;
        ~HttpServerModule() override
        {
            (void)shutdown();
        }

        std::string getName() const override
        {
            return "HttpServerModule";
        }
        std::expected<void, CentralPlatform::Core::AppError> init() override
        {
            CROW_ROUTE(app, "/api/status")([]()
                                           { return crow::response(200, R"({"status": "Engine is running"})"); });
            std::cout << "Server starting on port 8080" << std::endl;
            serverThread = std::thread([this]()
                                       { app.port(8080).multithreaded().run(); });
            return {};
        }
        std::expected<void, CentralPlatform::Core::AppError> shutdown() override
        {
            std::cout << "HttpServerModule shutting down" << std::endl;
            app.stop();
            if (serverThread.joinable())
            {
                serverThread.join();
            }
            return {};
        }
    };
}