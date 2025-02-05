/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
    class PhysicalDevice;
    class Surface;

    /*  Allows to hide boilerplate code when it is needed
        to determine whether some format or memory features
        are supported by physical device or not. */

    class DeviceFeatures : NonCopyable
    {
    public:
        struct FormatFeatures;
        struct ExternalFeatures;
        struct ExternalMemoryFeatures;
        enum class Vendor : uint32_t;

        bool isUmaArchitecture() const noexcept;
        FormatFeatures supportsFormatFeatures(VkFormat format,
            VkFormatFeatureFlags flags) const noexcept;
    #ifdef VK_KHR_external_memory_capabilities
        ExternalMemoryFeatures supportsExternalBuffer(VkExternalMemoryHandleTypeFlagBitsKHR handleType,
            VkBufferUsageFlags usage,
            VkBufferCreateFlags flags = 0) const;
        ExternalMemoryFeatures supportsExternalImage(VkExternalMemoryHandleTypeFlagBitsKHR handleType,
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
        bool supportsImageUsage(lent_ptr<const Surface> surface,
            VkImageUsageFlags flags) const;
        bool supportsDeviceLocalHostVisibleMemory() const noexcept;
        Vendor getVendor() const noexcept;

    private:
        MAGMA_MAKE_UNIQUE(DeviceFeatures)
        DeviceFeatures(const PhysicalDevice *physicalDevice) noexcept;
        friend PhysicalDevice;

        const PhysicalDevice *physicalDevice;
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

    enum class DeviceFeatures::Vendor : uint32_t
    {
        AMD, Apple, ARM, Broadcom, ImaginationTechnologies,
        Intel, Matrox, Nvidia, S3Graphics, Qualcomm,
        VeriSilicon, VIA, Unknown
    };
} // namespace magma
