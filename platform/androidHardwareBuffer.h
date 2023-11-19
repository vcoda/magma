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

namespace magma
{
    class Device;

    /* Enables an application to import Android AHardwareBuffer
       objects created outside of the Vulkan device into
       Vulkan memory objects, where they can be bound to
       images and buffers. */

#ifdef VK_ANDROID_external_memory_android_hardware_buffer
    class AndroidHardwareBuffer : NonCopyable
    {
    public:
        explicit AndroidHardwareBuffer(std::shared_ptr<Device> device,
            AHardwareBuffer* buffer);
        AHardwareBuffer* getBuffer() const noexcept { return buffer; }
        VkFormat getFormat() const noexcept { return formatProperties.format; }
        const VkAndroidHardwareBufferPropertiesANDROID& getProperties() const noexcept { return properties; }
        const VkAndroidHardwareBufferFormatPropertiesANDROID& getFormatProperties() const noexcept { return formatProperties; }
        VkExtent2D getExtent() const noexcept;
        VkImageUsageFlags getImageUsage() const noexcept;
        bool protectedContent() const noexcept;
        VkMemoryRequirements getMemoryRequirements() const noexcept;

    private:
        AHardwareBuffer* buffer;
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
        AHardwareBuffer_Desc bufferDesc;
    #endif
        VkAndroidHardwareBufferPropertiesANDROID properties;
        VkAndroidHardwareBufferFormatPropertiesANDROID formatProperties;
    };
#endif // VK_ANDROID_external_memory_android_hardware_buffer
} // namespace magma
