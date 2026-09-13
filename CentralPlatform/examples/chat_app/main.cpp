#include "../../Core/module_system/ModuleManager.h"
#include "../../modules/auth/Authmodle.h"

#include <iostream>
#include <memory>

int main()
{
    CentralPlatform::Core::ModuleManager manager;

    manager.registerModule(std::make_unique<CentralPlatform::Modules::Auth::AuthModule>());

    auto result = manager.initializeAll();

    if (result.has_value())
    {
        std::cout << "chat app , engine sarted" << std::endl;
    }
    else
    {
        std::cerr << "chat app , engine failed";
        return 1;
    }
    manager.shutdownAll();
    return 0;
}