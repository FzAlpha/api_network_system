#pragma once

#include "../../Core/module_system/IModule.h"
#include "../../Core/module_system/ModuleManager.h"
#include "../../providers/database/interface/IDatabaseProvider.h"
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
      std::cout << "AuthModel initializing ...\n";
      auto db = m_manager.getProvider<CentralPlatform::Providers::Database::IDatabaseProvider>();
      if (!db)
      {
        std::cerr << "Authmodel error : DAtabase provider not found" << std::endl;
        return std::unexpected(CentralPlatform::Core::AppError(CentralPlatform::Core::ErrorType::Internal, 500, "DB Missing"));
      }
      std::cout << "AuthModel successfully recieved the database provider" << std::endl;
      return {};
    }

    std::expected<void, CentralPlatform::Core::AppError> shutdown() override
    {
      std::cout << "AuthModel shutting down ..";
      return {};
    }
  };
} // namespace CentralPlatform::Modules::Auth
