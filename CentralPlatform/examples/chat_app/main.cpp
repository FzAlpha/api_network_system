#include "../../Core/module_system/ModuleManager.h"
#include "../../modules/auth/AuthModule.h"
#include "../../api/gateway/HttpServerModule.h"
#include "../../providers/database/sqlite/sqlprovider.h"

#include <iostream>
#include <memory>

int main()
{
    CentralPlatform::Core::ModuleManager manager;

    manager.registerModule(std::make_unique<CentralPlatform::Modules::Auth::AuthModule>());
    manager.registerModule(std::make_unique<CentralPlatform::Modules::ApiGateway::HttpServerModule>());

    auto result = manager.initializeAll();

    if (!result.has_value())
    {
        std::cerr << "chat app, engine failed to start" << std::endl;
        return 1;
    }

    std::cout << "chat app, engine started successfully!" << std::endl;

    // Database Provider Test
    std::unique_ptr<CentralPlatform::Providers::Database::IDatabaseProvider> db =
        std::make_unique<CentralPlatform::Providers::Database::sqllite::sqlProvider>();

    auto connectResult = db->connect("chat_users.db");
    if (!connectResult.has_value())
    {
        std::cerr << "Database connection failed: " << connectResult.error().message << std::endl;
    }
    else
    {
        // 1. Create Table
        auto createResult = db->execute("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT, email TEXT);");
        if (!createResult.has_value())
        {
            std::cerr << "Create table failed: " << createResult.error().message << std::endl;
        }

        // 2. Insert Data
        auto insertResult = db->execute("INSERT INTO users (username, email) VALUES ('AdminUser', 'admin@chat.com');");
        if (!insertResult.has_value())
        {
            std::cerr << "Insert data failed: " << insertResult.error().message << std::endl;
        }

        // 3. Query Data
        auto result = db->execute("SELECT * FROM users;");

        // 4. Print Result
        if (result.has_value())
        {
            if (!result->rows.empty() && result->rows[0].size() > 2)
            {
                std::cout << "Fetched User: " << result->rows[0][1] << " | " << result->rows[0][2] << std::endl;
            }
            else
            {
                std::cout << "Query succeeded, but no user rows returned." << std::endl;
            }
        }
        else
        {
            std::cerr << "Query execution failed: " << result.error().message << std::endl;
        }

        db->disconnect();
    }

    std::cout << "Press Enter to stop the server..." << std::endl;
    std::cin.get();

    manager.shutdownAll();
    return 0;
}