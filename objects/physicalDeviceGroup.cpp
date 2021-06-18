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
    MAGMA_STACK_ARRAY(VkPhysicalDevice, dereferencedPhysicalDevices, physicalDeviceCount());
    for (const auto& physicalDevice : physicalDevices)
        dereferencedPhysicalDevices.put(*physicalDevice);
    VkDeviceGroupDeviceCreateInfo groupInfo;
    groupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO;
    groupInfo.pNext = nullptr;
    groupInfo.physicalDeviceCount = physicalDeviceCount();
    groupInfo.pPhysicalDevices = dereferencedPhysicalDevices;
    std::vector<void *> extendedDeviceGroupFeatures = extendedDeviceFeatures;
    extendedDeviceGroupFeatures.push_back(&groupInfo);
    return physicalDevices.front()->createDevice(queueDescriptors,
        layers, extensions,
        deviceFeatures, extendedDeviceGroupFeatures);
}
#endif // VK_KHR_device_group
} // namespace magma
