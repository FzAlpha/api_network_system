#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include "IModule.h"

namespace CentralPlatform::Core
{
    class ModuleManager
    {
    private:
        std::vector<std::unique_ptr<IModule>> modules;

    public:
        ModuleManager() = default;
        ~ModuleManager() = default;

        void registerModule(std::unique_ptr<IModule> module)
        {
            modules.push_back(std::move(module));
        }
        std::expected<void, AppError> initializeAll()
        {
            for (auto &mod : modules)
            {
                auto result = mod->init();

                if (!result.has_value())
                {
                    return std::unexpected(result.error());
                }
            }
            return {};
        }
        void shutdownAll()
        {
            for (auto it = modules.rbegin(); it != modules.rend(); ++it)
            {
                auto result = (*it)->shutdown();

                if (!result.has_value())
                {
                    std::cerr << "warning failed to shutdown :" << (*it)->getName() << std::endl;
                }
            }
            modules.clear();
        }
    };
}