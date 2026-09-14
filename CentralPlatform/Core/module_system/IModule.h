#pragma once
#include "../errors/apperrors.h"

#include <expected>
#include <string>

namespace CentralPlatform::Core
{
  class IModule
  {
  public:
    virtual ~IModule() {}

    virtual std::string getName() const = 0;
    virtual std::expected<void, AppError> init() = 0;
    virtual std::expected<void, AppError> shutdown() = 0;
  };
} // namespace CentralPlatform::Core
