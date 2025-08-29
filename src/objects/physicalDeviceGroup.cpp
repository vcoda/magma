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
#include "pch.h"
#pragma hdrstop
#include "physicalDeviceGroup.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"

namespace magma
{
#ifdef VK_KHR_device_group
PhysicalDeviceGroup::PhysicalDeviceGroup(std::vector<std::shared_ptr<PhysicalDevice>> physicalDevices, uint32_t groupId) noexcept:
    physicalDevices(std::move(physicalDevices)),
    groupId(groupId)
{}

std::shared_ptr<Device> PhysicalDeviceGroup::createDevice(const std::set<DeviceQueueDescriptor>& queueDescriptors,
    const NullTerminatedStringArray& enabledLayers, const NullTerminatedStringArray& enabledExtensions,
    const VkPhysicalDeviceFeatures& enabledFeatures,
    const StructureChain& enabledExtendedFeatures /* default */,
    const StructureChain& extendedInfo /* default */) const
{
    auto dereferencedPhysicalDevices = stackalloc(VkPhysicalDevice, physicalDevices.size());
    uint32_t physicalDeviceCount = 0;
    for (auto const& physicalDevice: physicalDevices)
        dereferencedPhysicalDevices[physicalDeviceCount++] = *physicalDevice;
    VkDeviceGroupDeviceCreateInfoKHR deviceGroupDeviceInfo;
    deviceGroupDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO;
    deviceGroupDeviceInfo.pNext = nullptr;
    deviceGroupDeviceInfo.physicalDeviceCount = physicalDeviceCount;
    deviceGroupDeviceInfo.pPhysicalDevices = dereferencedPhysicalDevices;
    StructureChain deviceGroupInfo(extendedInfo);
    deviceGroupInfo.linkNode(deviceGroupDeviceInfo);
    return physicalDevices.front()->createDevice(queueDescriptors,
        enabledLayers, enabledExtensions,
        enabledFeatures, enabledExtendedFeatures,
        deviceGroupInfo);
}
#endif // VK_KHR_device_group
} // namespace magma
