#pragma once

#include "../../Core/module_system/IModule.h"
#include <iostream>

namespace CentralPlatform::Modules::Auth
{
  class AuthModule : public CentralPlatform::Core::IModule
  {
  public:
    AuthModule() = default;
    ~AuthModule() override = default;

    std::string getName() const override { return "AuthModule"; }

    std::expected<void, CentralPlatform::Core::AppError> init() override
    {
      std::cout << "AuthModel initializing ...\n";
      return {};
    }

    std::expected<void, CentralPlatform::Core::AppError> shutdown() override
    {
      std::cout << "AuthModel shutting down ..";
      return {};
    }
  };
} // namespace CentralPlatform::Modules::Auth
