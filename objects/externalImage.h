/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "image2D.h"

namespace magma
{
#ifdef VK_ANDROID_external_memory_android_hardware_buffer
    class AndroidHardwareBuffer;
#endif

    /* Image bound to external memory such as Android hardware buffer
       or POSIX file descriptor. */

    class ExternalImage2D : public Image2D
    {
    public:
    #ifdef VK_ANDROID_external_memory_android_hardware_buffer
        explicit ExternalImage2D(std::shared_ptr<Device> device,
            std::shared_ptr<AndroidHardwareBuffer> hardwareBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
    #endif // VK_ANDROID_external_memory_android_hardware_buffer
    };
} // namespace magma
