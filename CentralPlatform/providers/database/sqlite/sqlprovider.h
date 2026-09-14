#pragma once
#include "../interface/IDatabaseProvider.h"
#include <iostream>
#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>

namespace CentralPlatform::Providers::Database::sqllite
{
    class sqlProvider : public IDatabaseProvider
    {
        std::unique_ptr<SQLite::Database> db;
        std::string m_connectionString;

    public:
        sqlProvider() = default;
        ~sqlProvider() override = default;
        // {
        //     disconnect();
        // }

        std::expected<void, CentralPlatform::Core::AppError> connect(const std::string &connectionString) override
        {
            try
            {
                disconnect();
                m_connectionString = connectionString;
                db = std::make_unique<SQLite::Database>(
                    m_connectionString,
                    SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
                std::cout << "SQL provider successfully connected to " << m_connectionString << std::endl;
                return {};
            }
            catch (const SQLite::Exception &e)
            {
                std::cerr << "SQLite connection error: " << e.what() << "\n";
                return std::unexpected(CentralPlatform::Core::AppError(CentralPlatform::Core::ErrorType::Internal, 500, e.what()));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Database error: " << e.what() << "\n";
                return std::unexpected(CentralPlatform::Core::AppError(CentralPlatform::Core::ErrorType::Internal, 500, e.what()));
            }
        }

        void disconnect() override
        {
            if (db)
            {
                db.reset();
                std::cout << "SQL provider database closed" << std::endl;
            }
        }

        std::expected<QueryResult, CentralPlatform::Core::AppError> execute(const std::string &sqlQuery) override
        {
            if (!db)
            {
                return std::unexpected(CentralPlatform::Core::AppError(CentralPlatform::Core::ErrorType::Internal, 500, "database not connected"));
            }
            try
            {
                QueryResult result;
                SQLite::Statement query(*db, sqlQuery);
                while (query.executeStep())
                {
                    std::vector<std::string> rowdata;
                    int columnCount = query.getColumnCount();
                    rowdata.reserve(columnCount);
                    for (int i = 0; i < columnCount; i++)
                    {
                        rowdata.push_back(query.getColumn(i).getText());
                    }
                    result.rows.push_back(std::move(rowdata));
                }
                result.affectedRows = db->getChanges();
                return result;
            }
            catch (const SQLite::Exception &e)
            {
                std::cerr << "SQLite query error: " << e.what() << std::endl;
                return std::unexpected(CentralPlatform::Core::AppError(CentralPlatform::Core::ErrorType::Internal, 500, e.what()));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Query error: " << e.what() << std::endl;
                return std::unexpected(CentralPlatform::Core::AppError(CentralPlatform::Core::ErrorType::Internal, 500, e.what()));
            }
        }
    };
}