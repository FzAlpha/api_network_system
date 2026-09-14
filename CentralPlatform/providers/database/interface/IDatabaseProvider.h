#pragma once

#include "Core/errors/apperrors.h"

#include <expected>
#include <string>
#include <vector>

namespace CentralPlatform::Providers::Database
{
    struct QueryResult
    {
        std::vector<std::vector<std::string>> rows;
        int affectedRows{0};
    };

    class IDatabaseProvider
    {
    public:
        virtual ~IDatabaseProvider() = default;

        virtual std::expected<void, CentralPlatform::Core::AppError> connect(const std::string &connectionString) = 0;
        virtual void disconnect() = 0;
        virtual std::expected<QueryResult, CentralPlatform::Core::AppError> execute(const std::string &sqlQuery) = 0;
    };
} // namespace CentralPlatform::Providers::Database