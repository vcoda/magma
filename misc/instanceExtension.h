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
#include "exception.h"
#include "../objects/instance.h"

namespace magma
{
    template<typename Func>
    class InstanceExtension
    {
    public:
        InstanceExtension(VkInstance instance,
            const char *name) noexcept;
        InstanceExtension(VkInstance instance,
            const char *name,
            const char *extension);
        operator Func() const noexcept
            { return reinterpret_cast<Func>(_addr); }

    private:
        static PFN_vkVoidFunction _addr;
    };
} // namespace magma

#define MAGMA_INSTANCE_EXTENSION(func, extension)\
    magma::InstanceExtension<PFN_##func> func(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(func), extension)
#define MAGMA_OPTIONAL_INSTANCE_EXTENSION(func)\
    magma::InstanceExtension<PFN_##func> func(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(func))

#include "instanceExtension.inl"
