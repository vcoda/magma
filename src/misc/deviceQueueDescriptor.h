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
#include "../misc/structureChain.h"

namespace magma
{
    class PhysicalDevice;

    /* The priority of each queue is a normalized floating-point
       value between 0.0 and 1.0, which is then translated to a
       discrete priority level by the implementation. Higher values
       indicate a higher priority, with 0.0 being the lowest priority
       and 1.0 being the highest. */

    constexpr float QueuePriorityLowest = 0.f;
    constexpr float QueuePriorityLow = 0.25f;
    constexpr float QueuePriorityDefault = 0.5f;
    constexpr float QueuePriorityHigh = 0.75f;
    constexpr float QueuePriorityHighest = 1.f;

    /* Describes the queue that is requested to be created
       along with the logical device. */

    class DeviceQueueDescriptor final : public VkDeviceQueueCreateInfo
    {
    public:
        explicit DeviceQueueDescriptor(lent_ptr<const PhysicalDevice> physicalDevice,
            VkQueueFlagBits capabilities,
            float queuePriority,
            VkDeviceQueueCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit DeviceQueueDescriptor(lent_ptr<const PhysicalDevice> physicalDevice,
            VkQueueFlagBits capabilities,
            const std::initializer_list<float>& queuePriorities = {QueuePriorityDefault},
            VkDeviceQueueCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
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
