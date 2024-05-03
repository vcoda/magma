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
#include "pch.h"
#pragma hdrstop
#include "privateDataSlot.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_EXT_private_data
PrivateDataSlot::PrivateDataSlot(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPrivateDataSlotCreateFlagsEXT flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT, std::move(device), std::move(allocator))
{
    VkPrivateDataSlotCreateInfoEXT privateDataSlotInfo;
    privateDataSlotInfo.sType = VK_STRUCTURE_TYPE_PRIVATE_DATA_SLOT_CREATE_INFO_EXT;
    privateDataSlotInfo.pNext = extendedInfo.headNode();
    privateDataSlotInfo.flags = flags;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreatePrivateDataSlotEXT, VK_EXT_PRIVATE_DATA_EXTENSION_NAME);
    const VkResult result = vkCreatePrivateDataSlotEXT(getNativeDevice(),
        &privateDataSlotInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create private data slot");
}

PrivateDataSlot::~PrivateDataSlot()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyPrivateDataSlotEXT);
    vkDestroyPrivateDataSlotEXT(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void PrivateDataSlot::setPrivateData(const IObject *object, uint64_t data)
{
    MAGMA_DEVICE_EXTENSION(vkSetPrivateDataEXT);
    const VkResult result = vkSetPrivateDataEXT(getNativeDevice(), object->getObjectType(), object->getObjectHandle(), handle, data);
    MAGMA_HANDLE_RESULT(result, "failed to set private data");
}

uint64_t PrivateDataSlot::getPrivateData(const IObject *object) const noexcept
{
    uint64_t data = 0ull;
    MAGMA_DEVICE_EXTENSION(vkGetPrivateDataEXT);
    vkGetPrivateDataEXT(getNativeDevice(), object->getObjectType(), object->getObjectHandle(), handle, &data);
    return data;
}
#endif // VK_EXT_private_data
} // namespace magma
