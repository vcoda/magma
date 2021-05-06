/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2021 Victor Coda.

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
#include "deviceMemoryAllocator.h"
#include "../objects/deviceMemory.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/instance.h"
#include "../objects/commandBuffer.h"
#include "../exceptions/errorResult.h"

#ifdef _MSC_VER
#pragma warning(disable : 4100 4127)
#endif
#define VMA_IMPLEMENTATION
#include "../third-party/VulkanMemoryAllocator/include/vk_mem_alloc.h"

static_assert(sizeof(magma::MemoryBudget) == sizeof(VmaBudget),
    "VmaBudget structure size mismatch");
static_assert(sizeof(magma::DefragmentationStats) == sizeof(VmaDefragmentationStats),
    "VmaDefragmentationStats structure size mismatch");

namespace magma
{
DeviceMemoryAllocator::DeviceMemoryAllocator(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> hostAllocator /* nullptr */):
    device(std::move(device)),
    hostAllocator(std::move(hostAllocator)),
    allocator(VK_NULL_HANDLE),
    defragmentationContext(VK_NULL_HANDLE)
{
    std::shared_ptr<PhysicalDevice> physicalDevice = this->device->getPhysicalDevice();
    VmaAllocatorCreateInfo allocatorInfo;
    allocatorInfo.flags = 0;
    allocatorInfo.physicalDevice = *physicalDevice;
    allocatorInfo.device = *this->device;
    allocatorInfo.preferredLargeHeapBlockSize = 0;
    allocatorInfo.pAllocationCallbacks = this->hostAllocator.get();
    allocatorInfo.pDeviceMemoryCallbacks = nullptr;
    allocatorInfo.frameInUseCount = 0;
    allocatorInfo.pHeapSizeLimit = nullptr;
    allocatorInfo.pVulkanFunctions = nullptr;
    allocatorInfo.pRecordSettings = nullptr;
    allocatorInfo.instance = *physicalDevice->getInstance();
    allocatorInfo.vulkanApiVersion = physicalDevice->getInstance()->getApiVersion();
    const VkResult result = vmaCreateAllocator(&allocatorInfo, &allocator);
    MAGMA_THROW_FAILURE(result, "failed to create VMA allocator");
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
    vmaDestroyAllocator(allocator);
}

DeviceMemoryBlock DeviceMemoryAllocator::alloc(const VkMemoryRequirements& memoryRequirements,
    VkMemoryPropertyFlags flags, const void *handle, SuballocationType suballocType)
{
    MAGMA_ASSERT(handle);
    VmaAllocationCreateInfo allocInfo;
    allocInfo.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
    if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {   // Memory that is both mappable on host and preferably fast to access by GPU
        if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU; // PCI "pinned" memory
        else // Memory will be used on device only, so fast access from the device is preferred
            allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    } 
    else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT && !(flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT))
        {   // Usage: staging copy of resources used as transfer source
            allocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
        } 
        else if (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
        {   // Memory mappable on host and cached, can be used for readback from GPU
            allocInfo.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
        }
        else
        {   // Unknown usage scenario
            allocInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;
        }
    } 
    else if (flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
    {   // Exists mostly on mobile platforms, always created as dedicated allocation
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
    } 
    else
    {   // VK_MEMORY_PROPERTY_PROTECTED_BIT
        // VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD
        // VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD
        allocInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;
    }
    if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    else
        allocInfo.requiredFlags = 0;
    if (VMA_MEMORY_USAGE_CPU_TO_GPU == allocInfo.usage)
        allocInfo.preferredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    else
        allocInfo.preferredFlags = 0;
    allocInfo.memoryTypeBits = 0;
    allocInfo.pool = VK_NULL_HANDLE;
    allocInfo.pUserData = nullptr;
    allocInfo.priority = 0.f;
    VmaAllocation allocation;
    VkResult result;
    switch (suballocType)
    {
    case SuballocationType::Buffer:
        result = vmaAllocateMemoryForBuffer(allocator, *reinterpret_cast<const VkBuffer *>(handle), &allocInfo, &allocation, nullptr);
        break;
    case SuballocationType::Image:
        result = vmaAllocateMemoryForImage(allocator, *reinterpret_cast<const VkImage *>(handle), &allocInfo, &allocation, nullptr);
        break;
    default: // SuballocationType::Unknown
        result = vmaAllocateMemory(allocator, &memoryRequirements, &allocInfo, &allocation, nullptr);
    }
    MAGMA_THROW_FAILURE(result, "failed to allocate memory");
    return reinterpret_cast<DeviceMemoryBlock>(allocation);
}

std::vector<DeviceMemoryBlock> DeviceMemoryAllocator::allocPages(const std::vector<VkMemoryRequirements>& memoryRequirements,
    const std::vector<VkMemoryPropertyFlags>& memoryFlags)
{
    std::vector<VmaAllocationCreateInfo> allocInfos;
    allocInfos.reserve(memoryFlags.size());
    for (const VkMemoryPropertyFlags flags : memoryFlags)
    {
        VmaAllocationCreateInfo allocInfo;
        allocInfo.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
        if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        {   // Memory that is both mappable on host and preferably fast to access by GPU
            if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
                allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU; // PCI "pinned" memory
            else // Memory will be used on device only, so fast access from the device is preferred
                allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        } 
        else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        {
            if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT && !(flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT))
            {   // Usage: staging copy of resources used as transfer source
                allocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
            }
            else if (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
            {   // Memory mappable on host and cached, can be used for readback from GPU
                allocInfo.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
            }
            else
            {   // Unknown usage scenario
                allocInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;
            }
        }
        else if (flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
        {   // Exists mostly on mobile platforms, always created as dedicated allocation
            allocInfo.usage = VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
        }
        else
        {   // VK_MEMORY_PROPERTY_PROTECTED_BIT
            // VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD
            // VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD
            allocInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;
        }
        if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
            allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        else
            allocInfo.requiredFlags = 0;
        if (VMA_MEMORY_USAGE_CPU_TO_GPU == allocInfo.usage)
            allocInfo.preferredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        else
            allocInfo.preferredFlags = 0;
        allocInfo.memoryTypeBits = 0;
        allocInfo.pool = VK_NULL_HANDLE;
        allocInfo.pUserData = nullptr;
        allocInfo.priority = 0.f;
        allocInfos.push_back(allocInfo);
    }
    std::vector<VmaAllocation> allocations(MAGMA_COUNT(allocInfos));
    const VkResult result = vmaAllocateMemoryPages(allocator, memoryRequirements.data(), allocInfos.data(),
        allocations.size(), allocations.data(), nullptr);
    MAGMA_THROW_FAILURE(result, "failed to allocate memory pages");
    std::vector<DeviceMemoryBlock> memoryPages;
    memoryPages.reserve(allocations.size());
    for (auto& allocation : allocations)
        memoryPages.push_back(reinterpret_cast<DeviceMemoryBlock>(allocation));
    return memoryPages;
}

DeviceMemoryBlock DeviceMemoryAllocator::realloc(DeviceMemoryBlock memory, VkDeviceSize size)
{
    MAGMA_THROW_FAILURE(VK_ERROR_FEATURE_NOT_PRESENT, "vmaResizeAllocation() deprecated");
    return nullptr;
}

void DeviceMemoryAllocator::free(DeviceMemoryBlock allocation) noexcept
{   
    vmaFreeMemory(allocator, reinterpret_cast<VmaAllocation>(allocation));
}

void DeviceMemoryAllocator::freePages(std::vector<DeviceMemoryBlock>& allocations) noexcept
{
    if (!allocations.empty())
    {
        vmaFreeMemoryPages(allocator, MAGMA_COUNT(allocations), reinterpret_cast<VmaAllocation *>(allocations.data()));
        allocations.clear();
        allocations.shrink_to_fit();
    }
}

VkDeviceMemory DeviceMemoryAllocator::getMemoryHandle(DeviceMemoryBlock allocation) const noexcept
{
    VmaAllocationInfo allocInfo = {};
    vmaGetAllocationInfo(allocator, reinterpret_cast<VmaAllocation>(allocation), &allocInfo);
    return allocInfo.deviceMemory;
}

std::vector<MemoryBudget> DeviceMemoryAllocator::getBudget() const noexcept
{
    VmaBudget budgets[VK_MAX_MEMORY_HEAPS];
    vmaGetBudget(allocator, budgets);
    const VkPhysicalDeviceMemoryProperties& memoryProperties = device->getPhysicalDevice()->getMemoryProperties();
    std::vector<MemoryBudget> heapBudgets(memoryProperties.memoryHeapCount);
    memcpy(heapBudgets.data(), budgets, sizeof(VmaBudget) * memoryProperties.memoryHeapCount);
    return heapBudgets;
}

VkResult DeviceMemoryAllocator::checkCorruption(uint32_t memoryTypeBits) noexcept
{
    return vmaCheckCorruption(allocator, memoryTypeBits);
}

VkResult DeviceMemoryAllocator::beginCpuDefragmentation(std::vector<DeviceMemoryBlock>& allocations,
    DefragmentationStats* stats /* nullptr */) noexcept
{
    VmaDefragmentationInfo2 cpuDefragInfo;
    cpuDefragInfo.flags = 0;
    cpuDefragInfo.allocationCount = MAGMA_COUNT(allocations);
    cpuDefragInfo.pAllocations = reinterpret_cast<VmaAllocation *>(allocations.data());
    cpuDefragInfo.pAllocationsChanged = nullptr;
    cpuDefragInfo.poolCount = 0;
    cpuDefragInfo.pPools = nullptr;
    cpuDefragInfo.maxCpuBytesToMove = VK_WHOLE_SIZE;
    cpuDefragInfo.maxCpuAllocationsToMove = std::numeric_limits<uint32_t>::max();
    cpuDefragInfo.maxGpuBytesToMove = 0;
    cpuDefragInfo.maxGpuAllocationsToMove = 0;
    cpuDefragInfo.commandBuffer = VK_NULL_HANDLE;
    return vmaDefragmentationBegin(allocator, &cpuDefragInfo, reinterpret_cast<VmaDefragmentationStats*>(stats), &defragmentationContext);
}

VkResult DeviceMemoryAllocator::beginGpuDefragmentation(std::shared_ptr<CommandBuffer> cmdBuffer, std::vector<DeviceMemoryBlock>& allocations,
    DefragmentationStats* stats /* nullptr */) noexcept
{
    VmaDefragmentationInfo2 gpuDefragInfo;
    gpuDefragInfo.flags = 0;
    gpuDefragInfo.allocationCount = MAGMA_COUNT(allocations);
    gpuDefragInfo.pAllocations = reinterpret_cast<VmaAllocation *>(allocations.data());
    gpuDefragInfo.pAllocationsChanged = nullptr;
    gpuDefragInfo.poolCount = 0;
    gpuDefragInfo.pPools = nullptr;
    gpuDefragInfo.maxCpuBytesToMove = 0;
    gpuDefragInfo.maxCpuAllocationsToMove = 0;
    gpuDefragInfo.maxGpuBytesToMove = VK_WHOLE_SIZE;
    gpuDefragInfo.maxGpuAllocationsToMove = std::numeric_limits<uint32_t>::max();
    gpuDefragInfo.commandBuffer = *cmdBuffer;
    return vmaDefragmentationBegin(allocator, &gpuDefragInfo, reinterpret_cast<VmaDefragmentationStats*>(stats), &defragmentationContext);
}

VkResult DeviceMemoryAllocator::endDefragmentation() noexcept
{
    return vmaDefragmentationEnd(allocator, defragmentationContext);
}

VkResult DeviceMemoryAllocator::map(DeviceMemoryBlock allocation, VkDeviceSize offset, void **data) noexcept
{
    MAGMA_ASSERT(data);
    *data = nullptr;
    const VkResult map = vmaMapMemory(allocator, reinterpret_cast<VmaAllocation>(allocation), data);
    if (VK_SUCCESS == map)
    {   // When succeeded, *ppData contains pointer to first byte of this memory,
        // so pointer should be offseted manually using <offset> parameter.
        *((uint8_t**)data) += offset;
    }
    return map;
}

void DeviceMemoryAllocator::unmap(DeviceMemoryBlock allocation) noexcept
{
    vmaUnmapMemory(allocator, reinterpret_cast<VmaAllocation>(allocation));
}

VkResult DeviceMemoryAllocator::flushMappedRange(DeviceMemoryBlock allocation, VkDeviceSize offset, VkDeviceSize size) noexcept
{
    vmaFlushAllocation(allocator, reinterpret_cast<VmaAllocation>(allocation), offset, size);
    return VK_SUCCESS;
}

VkResult DeviceMemoryAllocator::invalidateMappedRange(DeviceMemoryBlock allocation, VkDeviceSize offset, VkDeviceSize size) noexcept
{
    vmaInvalidateAllocation(allocator, reinterpret_cast<VmaAllocation>(allocation), offset, size);
    return VK_SUCCESS;
}
} // namespace magma
