/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../objects/instance.h"
#include "../exceptions/unsupportedExtension.h"

namespace magma
{
    template<typename Func>
    class InstanceExtension
    {
    public:
        explicit InstanceExtension(VkInstance instance,
            const char *name) noexcept;
        explicit InstanceExtension(VkInstance instance,
            const char *name,
            const char *extension,
            const exception::source_location& location);
        operator Func() const noexcept { return reinterpret_cast<Func>(pfn); }

    private:
        static PFN_vkVoidFunction pfn;
    };
} // namespace magma

#define MAGMA_INSTANCE_EXTENSION(func, extension) magma::InstanceExtension<PFN_##func> func(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(func), extension, MAGMA_SOURCE_LOCATION)
#define MAGMA_OPTIONAL_INSTANCE_EXTENSION(func) magma::InstanceExtension<PFN_##func> func(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(func))

#include "instanceExtension.inl"
