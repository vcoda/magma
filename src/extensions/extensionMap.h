/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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

namespace magma
{
    template<class Properties>
    class ExtensionMap
    {
    public:
        bool find(const char *name) const noexcept
        {
            return map.find(name) != map.end();
        }

    protected:
        ExtensionMap(const std::vector<Properties>& properties)
        {
            for (auto const& property: properties)
            {
                if constexpr (std::is_same<Properties, VkExtensionProperties>::value)
                    map.emplace(std::string(property.extensionName), property);
                else if constexpr (std::is_same<Properties, VkLayerProperties>::value)
                    map.emplace(std::string(property.layerName), property);
            }
        }

    private:
        std::map<std::string, Properties> map;
    };
} // namespace magma

#define MAGMA_CHECK_EXTENSION(name) name(find(MAGMA_EXTENSION_PREFIX #name))
