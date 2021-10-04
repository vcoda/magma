/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
#include "physicalDeviceGroup.h"
#include "physicalDevice.h"
#include "../misc/createInfo.h"
#include "../helpers/stackArray.h"

namespace magma
{
#ifdef VK_KHR_device_group
PhysicalDeviceGroup::PhysicalDeviceGroup(const std::vector<std::shared_ptr<PhysicalDevice>>& physicalDevices, uint32_t groupId) noexcept:
    physicalDevices(physicalDevices),
    groupId(groupId)
{}

std::shared_ptr<Device> PhysicalDeviceGroup::createDevice(const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& layers,
    const std::vector<const char *>& extensions,
    const VkPhysicalDeviceFeatures& deviceFeatures,
    const std::vector<void *>& extendedDeviceFeatures /* {} */) const
{
    class DeviceGroupCreateInfo : public CreateInfo
    {
    public:
        DeviceGroupCreateInfo(uint32_t physicalDeviceCount, const VkPhysicalDevice *physicalDevices)
        {
            deviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO;
            deviceGroupInfo.pNext = nullptr;
            deviceGroupInfo.physicalDeviceCount = physicalDeviceCount;
            deviceGroupInfo.pPhysicalDevices = physicalDevices;
        }

        const void *getNode() const noexcept override
        {
            return &deviceGroupInfo;
        }

    private:
        VkDeviceGroupDeviceCreateInfo deviceGroupInfo;
    };
    MAGMA_STACK_ARRAY(VkPhysicalDevice, dereferencedPhysicalDevices, physicalDeviceCount());
    for (const auto& physicalDevice : physicalDevices)
        dereferencedPhysicalDevices.put(*physicalDevice);
    // Create a logical device from multiple physical devices
    return physicalDevices.front()->createDevice(queueDescriptors,
        layers, extensions, deviceFeatures, extendedDeviceFeatures,
        DeviceGroupCreateInfo(physicalDeviceCount(), dereferencedPhysicalDevices));
}
#endif // VK_KHR_device_group
} // namespace magma
