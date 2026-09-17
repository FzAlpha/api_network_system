#pragma once

#include "../../Core/module_system/IModule.h"
#include "../../Core/module_system/ModuleManager.h"
#include "../../providers/database/interface/IDatabaseProvider.h"
#include "../../api/gateway/HttpServerModule.h"
#include <iostream>
#include <memory>

namespace CentralPlatform::Modules::Auth
{
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
        http->registerPostRoute("/api/register", [](const crow::request &req)
                                { (void)req; return crow::response(200, R"({"status": "success", "message": "Hit the auth register route!"})"); });
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
