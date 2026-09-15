#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include "IModule.h"

namespace CentralPlatform::Core
{
    class ModuleManager
    {
    private:
        std::vector<std::unique_ptr<IModule>> modules;
        std::unordered_map<size_t, std::shared_ptr<void>> providers;

    public:
        ModuleManager() = default;
        ~ModuleManager() = default;

        void registerModule(std::unique_ptr<IModule> module)
        {
            modules.push_back(std::move(module));
        }

        template <typename P>
        void registerProvider(std::shared_ptr<P> provider)
        {
            if (!provider)
                return;
            size_t typeID = typeid(P).hash_code();
            providers[typeID] = std::static_pointer_cast<void>(provider);
        }

        template <typename T>
        std::shared_ptr<T> getProvider()
        {
            size_t typeId = typeid(T).hash_code();
            auto it = providers.find(typeId);
            if (it != providers.end())
            {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
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