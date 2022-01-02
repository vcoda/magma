/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
#include "extensions.h"
#include "../core/constexprHash.h"

namespace magma
{
Extensions::Extensions(const std::vector<VkExtensionProperties>& properties)
{
    for (const auto& property : properties)
    {
        extensions.emplace(property.extensionName, property.specVersion);
        hashes.emplace(core::hashString(property.extensionName));
    }
}

bool Extensions::hasExtension(const char *name) const noexcept
{
    static const std::string prefix(MAGMA_EXTENSION_PREFIX);
    const std::size_t hash = core::hashString((prefix + name).c_str());
    return hasExtension(hash);
}

void Extensions::forEach(std::function<void(const std::string&, uint32_t)> fn) const noexcept
{
    for (const auto& ext : extensions)
        fn(ext.first, ext.second);
}
} // namespace magma
