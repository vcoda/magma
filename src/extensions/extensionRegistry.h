/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
    protected:
        ExtensionRegistry(const std::vector<Properties>& properties);
        bool available(uint32_t hash) const noexcept;

    private:
        std::vector<uint32_t> hashes;
    };
} // namespace magma

#include "extensionRegistry.inl"

#define MAGMA_QUERY_AVAILABILITY(name) available(MAGMA_COMPILE_TIME_STRING_HASH(name))

#define MAGMA_EXTENSION(name) name(MAGMA_QUERY_AVAILABILITY(MAGMA_EXTENSION_PREFIX #name))
#define MAGMA_LAYER(name) name(MAGMA_QUERY_AVAILABILITY(MAGMA_LAYER_PREFIX #name))
#define MAGMA_LAYER_UNPREFIXED(name) name(MAGMA_QUERY_AVAILABILITY(#name))
