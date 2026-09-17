#pragma once

#include "../../Core/module_system/IModule.h"
#include "../../Core/module_system/ModuleManager.h"
#include "../../providers/database/interface/IDatabaseProvider.h"
#include "../../api/gateway/HttpServerModule.h"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

namespace CentralPlatform::Modules::Auth
{
  using json = nlohmann::json;
  class AuthModule : public CentralPlatform::Core::IModule
  {
    CentralPlatform::Core::ModuleManager &m_manager;

  public:
    explicit AuthModule(CentralPlatform::Core::ModuleManager &manager) : m_manager(manager) {}
    ~AuthModule() override = default;

    std::string getName() const override { return "AuthModule"; }

    std::expected<void, CentralPlatform::Core::AppError> init() override
    {
      std::cout << "AuthModule initializing ...\n";
      auto db = m_manager.getProvider<CentralPlatform::Providers::Database::IDatabaseProvider>();
      if (!db)
      {
        std::cerr << "AuthModule error: Database provider not found" << std::endl;
        return std::unexpected(CentralPlatform::Core::AppError(CentralPlatform::Core::ErrorType::Internal, 500, "DB Missing"));
      }
      std::cout << "AuthModule successfully received the database provider" << std::endl;

      auto *http = m_manager.getModule<CentralPlatform::Modules::ApiGateway::HttpServerModule>();
      if (http)
      {
        http->registerPostRoute("/api/register", [db](const crow::request &req) -> crow::response
                                {
          json parsed_data;

          try
          {
            parsed_data = json::parse(req.body);
            if (!parsed_data.contains("username") || !parsed_data["username"].is_string() ||
                !parsed_data.contains("email") || !parsed_data["email"].is_string())
            {
              return crow::response(400, R"({"error":"Missing or invalid 'username' or 'email'"})");
            }
          }
          catch (const json::exception &e)
          {
            return crow::response(400, R"({"error":"Invalid JSON"})");
          }

          std::string username = parsed_data["username"];
          std::string email = parsed_data["email"];
          std::string query = "INSERT INTO users (username,email) VALUES ('" + username + "','" + email + "');";

          auto result = db->execute(query);
          if (!result.has_value())
          {
            return crow::response(500, R"({"error":"Database insertion failed"})");
          }

          json response_body;
          response_body["status"] = "success";
          response_body["message"] = "User registered successfully";
          return crow::response(200, response_body.dump()); });
      }
      else
      {
        std::cerr << "AuthModule warning: HttpServerModule not found" << std::endl;
      }

      return {};
    }

    std::expected<void, CentralPlatform::Core::AppError> shutdown() override
    {
      std::cout << "AuthModule shutting down ...\n";
      return {};
    }
  };
} // namespace CentralPlatform::Modules::Auth
