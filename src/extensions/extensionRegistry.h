/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
    /* A registry of instance- or device-level extensions. */

    template<class Properties>
    class ExtensionRegistry
    {
    public:
        uint32_t getCount() const noexcept { return count; }
        uint32_t getSupportedCount() const noexcept { return supportedCount; }

    protected:
        ExtensionRegistry(const std::vector<Properties>& properties);
        bool supported(const char *name) noexcept;

    private:
        std::map<std::string, Properties> registry;
        uint32_t count;
        uint32_t supportedCount;
    };
} // namespace magma

#include "extensionRegistry.inl"

#define MAGMA_CHECK_EXTENSION(name) name(supported(MAGMA_EXTENSION_PREFIX #name))
