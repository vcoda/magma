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

    /*  Utility class that allows to determine whether some
        extended device features have been enabled or not. */

    class DeviceFeatures : NonCopyable
    {
    public:
        struct FormatFeatures;
        struct ExternalFenceFeatures;
        struct ExternalSemaphoreFeatures;

        FormatFeatures supportsFormatFeatures(VkFormat format,
            VkFormatFeatureFlags flags) const noexcept;
    #ifdef VK_KHR_external_fence_capabilities
        ExternalFenceFeatures supportsExternalFence(VkExternalFenceHandleTypeFlagBitsKHR handleType) const;
    #endif
    #ifdef VK_KHR_external_semaphore_capabilities
        ExternalSemaphoreFeatures supportsExternalSemaphore(VkExternalSemaphoreHandleTypeFlagBitsKHR handleType) const;
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

        std::weak_ptr<const Device> parent;
    };

    struct DeviceFeatures::FormatFeatures
    {
        VkBool32 linear: 1;
        VkBool32 optimal: 1;
        VkBool32 buffer: 1;
    };

    struct DeviceFeatures::ExternalFenceFeatures
    {
        VkBool32 exportable: 1;
        VkBool32 importable: 1;
    };

    struct DeviceFeatures::ExternalSemaphoreFeatures
    {
        VkBool32 exportable: 1;
        VkBool32 importable: 1;
    };
} // namespace magma
