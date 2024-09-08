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
    class PhysicalDevice;

    /* Describes the queue that is requested to be created
       along with the logical device. */

    class DeviceQueueDescriptor final : public VkDeviceQueueCreateInfo
    {
    public:
        explicit DeviceQueueDescriptor(std::shared_ptr<const PhysicalDevice> device,
            VkQueueFlagBits capabilities,
            const std::vector<float>& queuePriorities = {1.f});
        DeviceQueueDescriptor(const DeviceQueueDescriptor&);
        ~DeviceQueueDescriptor();
        DeviceQueueDescriptor& operator=(const DeviceQueueDescriptor&);
        bool operator<(const DeviceQueueDescriptor& other) const noexcept
            { return queueFamilyIndex < other.queueFamilyIndex; }

    private:
        std::optional<uint32_t> findDedicatedQueueFamily(
            const std::vector<VkQueueFamilyProperties>& properties,
            VkQueueFlags capability,
            VkQueueFlags excludeCapabilities = 0) const noexcept;
        std::optional<uint32_t> findAnySuitableQueueFamily(
            const std::vector<VkQueueFamilyProperties>& properties,
            VkQueueFlags capabilities) const noexcept;
    };
} // namespace magma
