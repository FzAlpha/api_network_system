#include "../../Core/module_system/ModuleManager.h"
#include "../../modules/auth/AuthModule.h"
#include "../../api/gateway/HttpServerModule.h"
#include "../../providers/database/sqlite/sqlprovider.h"

#include <iostream>
#include <memory>

int main()
{
    CentralPlatform::Core::ModuleManager manager;

    // Create SQLite provider as std::shared_ptr (for shared ownership in registry)
    auto db = std::make_shared<CentralPlatform::Providers::Database::sqllite::sqlProvider>();
    auto connectResult = db->connect("chat_users.db");
    if (!connectResult.has_value())
    {
        std::cerr << "Database connection failed: " << connectResult.error().message << std::endl;
        return 1;
    }

    // 1. Register provider in engine
    manager.registerProvider<CentralPlatform::Providers::Database::IDatabaseProvider>(db);

    // 2. Register modules (passing manager to AuthModule)
    manager.registerModule(std::make_unique<CentralPlatform::Modules::Auth::AuthModule>(manager));
    manager.registerModule(std::make_unique<CentralPlatform::Modules::ApiGateway::HttpServerModule>());

    auto result = manager.initializeAll();

    if (!result.has_value())
    {
        std::cerr << "chat app, engine failed to start" << std::endl;
        return 1;
    }

    std::cout << "chat app, engine started successfully!" << std::endl;

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
    auto queryResult = db->execute("SELECT * FROM users;");

    // 4. Print Result
    if (queryResult.has_value())
    {
        if (!queryResult->rows.empty() && queryResult->rows[0].size() > 2)
        {
            std::cout << "Fetched User: " << queryResult->rows[0][1] << " | " << queryResult->rows[0][2] << std::endl;
        }
        else
        {
            std::cout << "Query succeeded, but no user rows returned." << std::endl;
        }
    }
    else
    {
        std::cerr << "Query execution failed: " << queryResult.error().message << std::endl;
    }

    std::cout << "Press Enter to stop the server..." << std::endl;
    std::cin.get();

    manager.shutdownAll();
    db->disconnect();
    return 0;
}