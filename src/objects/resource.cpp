/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "resource.h"
#include "deviceMemory.h"
#include "managedDeviceMemory.h"
#include "../allocator/allocator.h"

namespace magma
{
std::unordered_map<NonDispatchableHandle, Resource*> Resource::resourceMap;
core::Spinlock Resource::mtx;

Resource::Resource(VkDeviceSize size, const Sharing& sharing) noexcept:
    sharing(sharing),
    size(size),
    offset(0ull)
{}

Resource *Resource::get(NonDispatchableHandle handle) noexcept
{
    std::lock_guard<core::Spinlock> lock(mtx);
    auto it = resourceMap.find(handle);
    if (it != resourceMap.end())
        return it->second;
    return nullptr;
}

void Resource::postCreate(NonDispatchableHandle handle)
{
    std::lock_guard<core::Spinlock> lock(mtx);
    resourceMap.emplace(handle, this);
}

void Resource::preDestroy(NonDispatchableHandle handle)
{
    std::lock_guard<core::Spinlock> lock(mtx);
    resourceMap.erase(handle);
}

std::shared_ptr<IDeviceMemory> Resource::allocateMemory(NonDispatchableHandle handle,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    const StructureChain& extendedMemoryInfo,
    std::shared_ptr<Device> device, std::shared_ptr<Allocator> allocator)
{
    std::shared_ptr<IAllocator> hostAllocator;
    std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator;
    if (allocator)
    {
        hostAllocator = allocator->getHostAllocator();
        deviceAllocator = allocator->getDeviceAllocator();
    }
    if (deviceAllocator)
    {
        VkObjectType objectType;
        if (getFamily() == IResource::Family::Buffer)
            objectType = VK_OBJECT_TYPE_BUFFER;
        else
            objectType = VK_OBJECT_TYPE_IMAGE;
        return std::make_unique<ManagedDeviceMemory>(std::move(device),
            objectType, handle, memoryRequirements, flags,
            std::move(hostAllocator), std::move(deviceAllocator),
            extendedMemoryInfo);
    }
    return std::make_shared<DeviceMemory>(std::move(device),
        memoryRequirements, flags, std::move(hostAllocator),
        extendedMemoryInfo);
}
} // namespace magma
