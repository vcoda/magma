/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "../core/constexprHash.h"

namespace magma
{
    /* For each extensions name we calculate its hash in compile-time.
       When instance or device extensions are enumerated, their hashes added to hash map,
       which allows fast lookup of extensions in ctor-initializer. */

    class Extensions
    {
    public:
        bool hasExtension(const char *name) const noexcept;

    protected:
        Extensions(const std::vector<VkExtensionProperties>&);
        inline bool hasExtension(std::size_t hash) const noexcept
        {
            return extensions.find(hash) != extensions.end();
        }

    private:
        std::unordered_set<std::size_t> extensions;
    };
} // namespace magma

#define MAGMA_EXTENSION_PREFIX "VK_"
#define MAGMA_EXTENSION(extension)\
    const bool extension;\
    static constexpr std::size_t __hash_of_ ## extension = magma::core::hashString(MAGMA_EXTENSION_PREFIX #extension);
#define MAGMA_CHECK_EXTENSION(extension) extension(hasExtension(__hash_of_ ## extension))
