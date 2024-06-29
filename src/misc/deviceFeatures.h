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
    class Surface;

    /*  Allows to hide boilerplate code when it is needed to determine
        whether some format or memory features are supported by device
        or not. Or whether some supported device features have been
        enabled or not. */

    class DeviceFeatures : NonCopyable
    {
    public:
        struct FormatFeatures;
        struct ExternalFeatures;
        struct ExternalMemoryFeatures;

        FormatFeatures supportsFormatFeatures(VkFormat format,
            VkFormatFeatureFlags flags) const noexcept;
    #ifdef VK_KHR_external_memory_capabilities
        ExternalMemoryFeatures supportsExternalBuffer(VkExternalMemoryHandleTypeFlagBits handleType,
            VkBufferUsageFlags usage,
            VkBufferCreateFlags flags = 0) const;
        ExternalMemoryFeatures supportsExternalImage(VkExternalMemoryHandleTypeFlagBits handleType,
            VkFormat format,
            VkImageUsageFlags usage,
            VkImageType imageType = VK_IMAGE_TYPE_2D,
            bool optimalTiling = true,
            VkImageCreateFlags flags = 0) const;
    #endif // VK_KHR_external_memory_capabilities
    #ifdef VK_KHR_external_fence_capabilities
        ExternalFeatures supportsExternalFence(VkExternalFenceHandleTypeFlagBitsKHR handleType) const;
    #endif
    #ifdef VK_KHR_external_semaphore_capabilities
        ExternalFeatures supportsExternalSemaphore(VkExternalSemaphoreHandleTypeFlagBitsKHR handleType) const;
    #endif
        bool supportsImageUsage(std::shared_ptr<const Surface> surface,
            VkImageUsageFlags flags) const;
        bool supportsDeviceLocalHostVisibleMemory() const noexcept;
        bool maintenanceEnabled(uint8_t index) const noexcept;
        bool negativeViewportHeightEnabled() const noexcept;
        bool separateDepthStencilLayoutsEnabled() const noexcept;
        bool extendedLinesEnabled() const noexcept;
        bool stippledLinesEnabled() const noexcept;

    private:
        MAGMA_MAKE_SHARED(DeviceFeatures)
        DeviceFeatures(std::shared_ptr<const Device> device) noexcept;
        friend Device;

        std::weak_ptr<const Device> owner;
    };

    struct DeviceFeatures::FormatFeatures
    {
        VkBool32 linear: 1;
        VkBool32 optimal: 1;
        VkBool32 buffer: 1;
    };

    struct DeviceFeatures::ExternalFeatures
    {
        VkBool32 exportable: 1;
        VkBool32 importable: 1;
    };

    struct DeviceFeatures::ExternalMemoryFeatures
    {
        VkBool32 dedicatedOnly: 1;
        VkBool32 exportable: 1;
        VkBool32 importable: 1;
    };
} // namespace magma
