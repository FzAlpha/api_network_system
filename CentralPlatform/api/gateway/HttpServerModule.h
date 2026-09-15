#pragma once
#include "../../Core/module_system/IModule.h"
#include <crow.h>
#include <thread>
#include <functional>
#include <iostream>
#include <atomic>
#include <cstdint>

namespace CentralPlatform::Modules::ApiGateway
{
    class HttpServerModule : public CentralPlatform::Core::IModule
    {
    public:
        using RouteHandler = std::function<crow::response(const crow::request &)>;
        using Routehandle = RouteHandler; // Alias for backward compatibility

    private:
        uint16_t port{8080};
        crow::SimpleApp app;
        std::thread serverThread;
        std::atomic<bool> isRunning{false};

    public:
        explicit HttpServerModule(uint16_t serverPort = 8080) : port(serverPort) {}
        ~HttpServerModule() override
        {
            if (auto res = shutdown(); !res.has_value())
            {
                std::cerr << "HttpServerModule: Error during shutdown in destructor: "
                          << res.error().message << std::endl;
            }
        }

        std::string getName() const override
        {
            return "HttpServerModule";
        }

        uint16_t getPort() const
        {
            return port;
        }

        uint16_t getport() const
        {
            return port;
        }

        bool registerGetRoute(const std::string &path, RouteHandler handler)
        {
            if (isRunning)
            {
                std::cerr << "HttpServerModule: Cannot register route after server is running -> " << path << std::endl;
                return false;
            }
            app.route_dynamic(path.c_str()).methods(crow::HTTPMethod::GET)(handler);
            std::cout << "HttpServerModule: Dynamically bound GET route -> " << path << std::endl;
            return true;
        }

        bool registerPostRoute(const std::string &path, RouteHandler handler)
        {
            if (isRunning)
            {
                std::cerr << "HttpServerModule: Cannot register route after server is running -> " << path << std::endl;
                return false;
            }
            app.route_dynamic(path.c_str()).methods(crow::HTTPMethod::POST)(handler);
            std::cout << "HttpServerModule: Dynamically bound POST route -> " << path << std::endl;
            return true;
        }

        std::expected<void, CentralPlatform::Core::AppError> init() override
        {
            if (isRunning)
            {
                return {};
            }

            registerGetRoute("/api/status", [](const crow::request &)
                             { return crow::response(200, R"({"status": "Engine is running"})"); });

            std::cout << "Server starting on port " << port << std::endl;
            try
            {
                serverThread = std::thread([this]()
                                           { app.port(port).multithreaded().run(); });
                app.wait_for_server_start();
                isRunning = true;
                std::cout << "Server started successfully on port " << port << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to start HttpServerModule: " << e.what() << std::endl;
                return std::unexpected(CentralPlatform::Core::AppError(
                    CentralPlatform::Core::ErrorType::Internal,
                    500,
                    std::string("HTTP server failed to start: ") + e.what()));
            }
            return {};
        }

        std::expected<void, CentralPlatform::Core::AppError> shutdown() override
        {
            if (!isRunning && !serverThread.joinable())
            {
                return {};
            }
            std::cout << "HttpServerModule shutting down" << std::endl;
            isRunning = false;
            app.stop();
            if (serverThread.joinable())
            {
                serverThread.join();
            }
            return {};
        }
    };
}