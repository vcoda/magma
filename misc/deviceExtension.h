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
#include "../exceptions/unsupportedExtension.h"

namespace magma
{
    template<typename Func>
    class DeviceExtension
    {
    public:
        DeviceExtension(VkDevice device,
            const char *name) noexcept;
        DeviceExtension(VkDevice device,
            const char *name,
            const char *extension,
            const exception::source_location& location);
        operator Func() const noexcept { return reinterpret_cast<Func>(pfn); }

    private:
        static PFN_vkVoidFunction pfn;
    };
} // namespace magma

#define MAGMA_DEVICE_EXTENSION(func, extension)\
    magma::DeviceExtension<PFN_##func> func(MAGMA_HANDLE(device), MAGMA_STRINGIZE(func), extension, magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__})
#define MAGMA_OPTIONAL_DEVICE_EXTENSION(func)\
    magma::DeviceExtension<PFN_##func> func(MAGMA_HANDLE(device), MAGMA_STRINGIZE(func))

#include "deviceExtension.inl"
