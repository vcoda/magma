/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <map>
#include "../objects/instance.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
    template<typename Function>
    class InstanceExtension
    {
    public:
        InstanceExtension(VkInstance instance, 
            const char *name, const char *extension,
            bool throwNotPresent)
        {
            MAGMA_ASSERT(name);
            auto it = cache.find(std::string(name));
            if (it != cache.end())
                pfn = reinterpret_cast<Function>(it->second);
            else
            {
                MAGMA_ASSERT(instance != VK_NULL_HANDLE);
                PFN_vkVoidFunction addr = vkGetInstanceProcAddr(instance, name);
                if (addr)
                {
                    cache[std::string(name)] = addr;
                    pfn = reinterpret_cast<Function>(addr);
                }
                else if (throwNotPresent)
                    MAGMA_THROW_NOT_PRESENT(extension);
            }
        }
        operator Function() { return pfn; }

    private:
        static std::map<std::string, PFN_vkVoidFunction> cache;
        Function pfn;
    };

    template<typename Function>
    std::map<std::string, PFN_vkVoidFunction> InstanceExtension<Function>::cache;
} // namespace magma

#define MAGMA_INSTANCE_EXTENSION(func, extension)\
    InstanceExtension<PFN_##func> func(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(func), extension, true)
#define MAGMA_OPTIONAL_INSTANCE_EXTENSION(func)\
    InstanceExtension<PFN_##func> func(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(func), nullptr, false)
