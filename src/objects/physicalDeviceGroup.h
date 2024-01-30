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
#include "../misc/deviceQueueDescriptor.h"
#include "../misc/structureChain.h"

namespace magma
{
    class Device;
    class PhysicalDevice;

    /* A device group can allocate memory across the subdevices,
       bind memory from one subdevice to a resource on another
       subdevice, record command buffers where some work executes
       on an arbitrary subset of the subdevices, and potentially
       present a swapchain image from one or more subdevices. */

#ifdef VK_KHR_device_group
    class PhysicalDeviceGroup
    {
    public:
        std::shared_ptr<Device> createDevice(const std::vector<DeviceQueueDescriptor>& queueDescriptors,
            const NullTerminatedStringArray& enabledLayers,
            const NullTerminatedStringArray& enabledExtensions,
            const VkPhysicalDeviceFeatures& enabledFeatures,
            const StructureChain& enabledExtendedFeatures = StructureChain(),
            const StructureChain& extendedInfo = StructureChain()) const;
        uint32_t getGroupId() const { return groupId; }
        uint32_t getPhysicalDeviceCount() const { return MAGMA_COUNT(physicalDevices); }
        const std::shared_ptr<PhysicalDevice>& getPhysicalDevice(uint32_t deviceId) const noexcept { return physicalDevices[deviceId]; }

    private:
        PhysicalDeviceGroup(const std::vector<std::shared_ptr<PhysicalDevice>>& physicalDevices,
            uint32_t groupId) noexcept;

        std::vector<std::shared_ptr<PhysicalDevice>> physicalDevices;
        const uint32_t groupId;
        friend class Instance;
    };
#endif // VK_KHR_device_group
} // namespace magma
