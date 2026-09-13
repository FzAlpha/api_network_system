#pragma once

#include <string>
#include <utility>

namespace CentralPlatform::Core {

enum class ErrorType {
    NotFound,
    ValidationFailed,
    Unauthorized,
    Internal
};

struct AppError {
    ErrorType type = ErrorType::Internal;
    int http_status = 500;
    std::string message;

    AppError() = default;
    AppError(ErrorType t, int status, std::string msg)
        : type(t), http_status(status), message(std::move(msg)) {}
};

} // namespace CentralPlatform::Core