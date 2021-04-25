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
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    device(std::move(device)),
    allocator(std::move(allocator)),
    handle(VK_NULL_HANDLE),
    defragmentationContext(VK_NULL_HANDLE)
{
    std::shared_ptr<PhysicalDevice> physicalDevice = this->device->getPhysicalDevice();
    VmaAllocatorCreateInfo allocatorInfo;
    allocatorInfo.flags = 0;
    allocatorInfo.physicalDevice = *physicalDevice;
    allocatorInfo.device = *this->device;
    allocatorInfo.preferredLargeHeapBlockSize = 0;
    allocatorInfo.pAllocationCallbacks = this->allocator.get();
    allocatorInfo.pDeviceMemoryCallbacks = nullptr;
    allocatorInfo.frameInUseCount = 0;
    allocatorInfo.pHeapSizeLimit = nullptr;
    allocatorInfo.pVulkanFunctions = nullptr;
    allocatorInfo.pRecordSettings = nullptr;
    allocatorInfo.instance = *physicalDevice->getInstance();
    allocatorInfo.vulkanApiVersion = physicalDevice->getInstance()->getApiVersion();
    const VkResult result = vmaCreateAllocator(&allocatorInfo, &handle);
    MAGMA_THROW_FAILURE(result, "failed to create VMA allocator");
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
    vmaDestroyAllocator(handle);
}

void *DeviceMemoryAllocator::alloc(const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    bool cpuFrequentlyWriteGpuRead)
{
    VmaAllocationCreateInfo allocInfo;
    allocInfo.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
    if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {   // Memory will be used on device only, so fast access from the device is preferred
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    } 
    else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT && !(flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT))
        {   // Memory that is both mappable on host and preferably fast to access by GPU
            if (cpuFrequentlyWriteGpuRead)
                allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
            else // Usage: staging copy of resources used as transfer source
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
    allocInfo.preferredFlags = 0;
    allocInfo.memoryTypeBits = 0;
    allocInfo.pool = VK_NULL_HANDLE;
    allocInfo.pUserData = nullptr;
    allocInfo.priority = 0.f;
    VmaAllocation allocation;
    const VkResult result = vmaAllocateMemory(handle, &memoryRequirements, &allocInfo, &allocation, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to allocate memory");
    return reinterpret_cast<void *>(allocation);
}

std::vector<void *> DeviceMemoryAllocator::allocPages(const std::vector<VkMemoryRequirements>& memoryRequirements,
    const std::vector<VkMemoryPropertyFlags>& memoryFlags)
{
    std::vector<VmaAllocationCreateInfo> allocInfos;
    allocInfos.reserve(memoryFlags.size());
    for (const VkMemoryPropertyFlags flags : memoryFlags)
    {
        VmaAllocationCreateInfo allocInfo;
        allocInfo.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
        if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        {   // Memory will be used on device only, so fast access from the device is preferred
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
        allocInfo.preferredFlags = 0;
        allocInfo.memoryTypeBits = 0;
        allocInfo.pool = VK_NULL_HANDLE;
        allocInfo.pUserData = nullptr;
        allocInfo.priority = 0.f;
        allocInfos.push_back(allocInfo);
    }
    const size_t allocationCount = MAGMA_COUNT(allocInfos);
    std::vector<void *> memoryPages(allocationCount);
    const VkResult result = vmaAllocateMemoryPages(handle, memoryRequirements.data(), allocInfos.data(), allocationCount,
        reinterpret_cast<VmaAllocation *>(memoryPages.data()), nullptr);
    MAGMA_THROW_FAILURE(result, "failed to allocate memory pages");
    return memoryPages;
}

void *DeviceMemoryAllocator::realloc(void *allocation, VkDeviceSize size)
{
    MAGMA_THROW_FAILURE(VK_ERROR_FEATURE_NOT_PRESENT, "vmaResizeAllocation() deprecated");
    return nullptr;
}

void DeviceMemoryAllocator::free(void *allocation) noexcept
{
    vmaFreeMemory(handle, reinterpret_cast<VmaAllocation>(allocation));
}

void DeviceMemoryAllocator::freePages(std::vector<void *>& memoryPages) noexcept
{
    if (!memoryPages.empty())
    {
        vmaFreeMemoryPages(handle, MAGMA_COUNT(memoryPages), reinterpret_cast<VmaAllocation *>(memoryPages.data()));
        memoryPages.clear();
        memoryPages.shrink_to_fit();
    }
}

VkDeviceMemory DeviceMemoryAllocator::getMemoryHandle(void *allocation) const noexcept
{
    VmaAllocationInfo allocInfo = {};
    vmaGetAllocationInfo(handle, reinterpret_cast<VmaAllocation>(allocation), &allocInfo);
    return allocInfo.deviceMemory;
}

VkResult DeviceMemoryAllocator::map(void *allocation, VkDeviceSize offset, void **data) noexcept
{
    MAGMA_ASSERT(data);
    *data = nullptr;
    const VkResult map = vmaMapMemory(handle, reinterpret_cast<VmaAllocation>(allocation), data);
    if (VK_SUCCESS == map)
    {   // When succeeded, *ppData contains pointer to first byte of this memory,
        // so pointer should be offseted manually using <offset> parameter.
        *((uint8_t**)data) += offset;
    }
    return map;
}

void DeviceMemoryAllocator::unmap(void *allocation) noexcept
{
    vmaUnmapMemory(handle, reinterpret_cast<VmaAllocation>(allocation));
}

VkResult DeviceMemoryAllocator::flushMappedRange(void *allocation, VkDeviceSize offset, VkDeviceSize size) noexcept
{
    vmaFlushAllocation(handle, reinterpret_cast<VmaAllocation>(allocation), offset, size);
    return VK_SUCCESS;
}

VkResult DeviceMemoryAllocator::invalidateMappedRange(void *allocation, VkDeviceSize offset, VkDeviceSize size) noexcept
{
    vmaInvalidateAllocation(handle, reinterpret_cast<VmaAllocation>(allocation), offset, size);
    return VK_SUCCESS;
}

std::vector<MemoryBudget> DeviceMemoryAllocator::getBudget() const noexcept
{
    VmaBudget budgets[VK_MAX_MEMORY_HEAPS];
    vmaGetBudget(handle, budgets);
    const VkPhysicalDeviceMemoryProperties& memoryProperties = device->getPhysicalDevice()->getMemoryProperties();
    std::vector<MemoryBudget> heapBudgets(memoryProperties.memoryHeapCount);
    memcpy(heapBudgets.data(), budgets, sizeof(VmaBudget) * memoryProperties.memoryHeapCount);
    return heapBudgets;
}

VkResult DeviceMemoryAllocator::checkCorruption(uint32_t memoryTypeBits) noexcept
{
    return vmaCheckCorruption(handle, memoryTypeBits);
}

VkResult DeviceMemoryAllocator::beginCpuDefragmentation(std::vector<void *>& allocations,
    DefragmentationStats* stats /* nullptr */) noexcept
{
    VmaDefragmentationInfo2 cpuDefragInfo;
    cpuDefragInfo.flags = 0;
    cpuDefragInfo.allocationCount = MAGMA_COUNT(allocations);
    cpuDefragInfo.pAllocations = reinterpret_cast<VmaAllocation*>(allocations.data());
    cpuDefragInfo.pAllocationsChanged = nullptr;
    cpuDefragInfo.poolCount = 0;
    cpuDefragInfo.pPools = nullptr;
    cpuDefragInfo.maxCpuBytesToMove = VK_WHOLE_SIZE;
    cpuDefragInfo.maxCpuAllocationsToMove = std::numeric_limits<uint32_t>::max();
    cpuDefragInfo.maxGpuBytesToMove = 0;
    cpuDefragInfo.maxGpuAllocationsToMove = 0;
    cpuDefragInfo.commandBuffer = VK_NULL_HANDLE;
    return vmaDefragmentationBegin(handle, &cpuDefragInfo, reinterpret_cast<VmaDefragmentationStats*>(stats), &defragmentationContext);
}

VkResult DeviceMemoryAllocator::beginGpuDefragmentation(std::shared_ptr<CommandBuffer> cmdBuffer, std::vector<void *>& allocations,
    DefragmentationStats* stats /* nullptr */) noexcept
{
    VmaDefragmentationInfo2 gpuDefragInfo;
    gpuDefragInfo.flags = 0;
    gpuDefragInfo.allocationCount = MAGMA_COUNT(allocations);
    gpuDefragInfo.pAllocations = reinterpret_cast<VmaAllocation*>(allocations.data());
    gpuDefragInfo.pAllocationsChanged = nullptr;
    gpuDefragInfo.poolCount = 0;
    gpuDefragInfo.pPools = nullptr;
    gpuDefragInfo.maxCpuBytesToMove = 0;
    gpuDefragInfo.maxCpuAllocationsToMove = 0;
    gpuDefragInfo.maxGpuBytesToMove = VK_WHOLE_SIZE;
    gpuDefragInfo.maxGpuAllocationsToMove = std::numeric_limits<uint32_t>::max();
    gpuDefragInfo.commandBuffer = *cmdBuffer;
    return vmaDefragmentationBegin(handle, &gpuDefragInfo, reinterpret_cast<VmaDefragmentationStats*>(stats), &defragmentationContext);
}

VkResult DeviceMemoryAllocator::endDefragmentation() noexcept
{
    return vmaDefragmentationEnd(handle, defragmentationContext);
}
} // namespace magma
