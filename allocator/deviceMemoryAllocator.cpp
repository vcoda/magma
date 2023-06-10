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
#include "../objects/managedDeviceMemory.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/instance.h"
#include "../objects/commandBuffer.h"
#include "../exceptions/errorResult.h"
#include "../core/forEach.h"
#include "../third-party/VulkanMemoryAllocator/include/vk_mem_alloc.h"

static_assert(sizeof(magma::MemoryBudget) == sizeof(VmaBudget),
    "VmaBudget structure size mismatch");
static_assert(sizeof(magma::DefragmentationStats) == sizeof(VmaDefragmentationStats),
    "VmaDefragmentationStats structure size mismatch");

namespace magma
{
DeviceMemoryAllocator::DeviceMemoryAllocator(std::shared_ptr<Device> device_,
    std::shared_ptr<IAllocator> hostAllocator_ /* nullptr */):
    device(std::move(device_)),
    hostAllocator(std::move(hostAllocator_)),
    allocator(VK_NULL_HANDLE),
    defragmentationContext(VK_NULL_HANDLE)
{
    std::shared_ptr<PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.flags = 0;
#if defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_dedicated_allocation)
    if (device->extensionEnabled(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME) &&
        device->extensionEnabled(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME))
    {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
    }
#endif // VK_KHR_get_memory_requirements2 && VK_KHR_dedicated_allocation
#ifdef VK_EXT_memory_priority
    if (device->extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT;
#endif // VK_EXT_memory_priority
    allocatorInfo.physicalDevice = *physicalDevice;
    allocatorInfo.device = *device;
    allocatorInfo.preferredLargeHeapBlockSize = 0;
    allocatorInfo.pAllocationCallbacks = hostAllocator.get();
    allocatorInfo.pDeviceMemoryCallbacks = nullptr;
    allocatorInfo.frameInUseCount = 0;
    allocatorInfo.pHeapSizeLimit = nullptr;
    allocatorInfo.pVulkanFunctions = nullptr;
    allocatorInfo.pRecordSettings = nullptr;
    allocatorInfo.instance = *physicalDevice->getInstance();
    allocatorInfo.vulkanApiVersion = physicalDevice->getInstance()->getApiVersion();
    allocatorInfo.pTypeExternalMemoryHandleTypes = nullptr;
    const VkResult result = vmaCreateAllocator(&allocatorInfo, &allocator);
    MAGMA_THROW_FAILURE(result, "failed to create VMA allocator");
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
    vmaDestroyAllocator(allocator);
}

DeviceMemoryBlock DeviceMemoryAllocator::allocate(VkObjectType objectType, NonDispatchableHandle object,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    const StructureChain& extendedInfo)
{
    VmaAllocationCreateInfo allocInfo;
    allocInfo.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
    allocInfo.usage = (VmaMemoryUsage)chooseMemoryUsage(flags);
    if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    else
        allocInfo.requiredFlags = 0;
    if (VMA_MEMORY_USAGE_CPU_TO_GPU == allocInfo.usage) // PCI "pinned" memory
        allocInfo.preferredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    else
        allocInfo.preferredFlags = 0;
    allocInfo.memoryTypeBits = 0;
    allocInfo.pool = VK_NULL_HANDLE;
    allocInfo.pUserData = nullptr;
    allocInfo.priority = MAGMA_DEFAULT_MEMORY_PRIORITY;
#ifdef VK_EXT_memory_priority
    if (device->extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
        VkMemoryPriorityAllocateInfoEXT *memoryPriorityAllocateInfo = extendedInfo.findNode<VkMemoryPriorityAllocateInfoEXT>();
        if (memoryPriorityAllocateInfo)
            allocInfo.priority = std::max(0.f, std::min(memoryPriorityAllocateInfo->priority, 1.f));
    }
#endif // VK_EXT_memory_priority
    VmaAllocation allocation;
    VkResult result;
    switch (objectType)
    {
    case VK_OBJECT_TYPE_BUFFER:
        result = vmaAllocateMemoryForBuffer(allocator, core::reinterpret<VkBuffer>(object), &allocInfo, &allocation, nullptr);
        break;
    case VK_OBJECT_TYPE_IMAGE:
        result = vmaAllocateMemoryForImage(allocator, core::reinterpret<VkImage>(object), &allocInfo, &allocation, nullptr);
        break;
    default:
        result = vmaAllocateMemory(allocator, &memoryRequirements, &allocInfo, &allocation, nullptr);
    }
    MAGMA_THROW_FAILURE(result, "failed to allocate memory");
    return reinterpret_cast<DeviceMemoryBlock>(allocation);
}

std::vector<DeviceMemoryBlock> DeviceMemoryAllocator::allocPages(const std::vector<VkMemoryRequirements>& memoryRequirements,
    const std::vector<VkMemoryPropertyFlags>& memoryFlags, const std::vector<float>& priorities)
{
    std::vector<VmaAllocationCreateInfo> allocInfos;
    allocInfos.reserve(memoryFlags.size());
    std::vector<float> defaultPriorities;
    if (priorities.empty())
        defaultPriorities.resize(memoryFlags.size(), MAGMA_DEFAULT_MEMORY_PRIORITY);
    core::forConstEach(memoryFlags, priorities.empty() ? defaultPriorities : priorities,
        [&allocInfos](auto& flags, auto& priority)
        {
            VmaAllocationCreateInfo allocInfo;
            allocInfo.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
            allocInfo.usage = (VmaMemoryUsage)chooseMemoryUsage(*flags);
            if (*flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
                allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            else if (*flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
                allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            else
                allocInfo.requiredFlags = 0;
            if (VMA_MEMORY_USAGE_CPU_TO_GPU == allocInfo.usage) // PCI "pinned" memory
                allocInfo.preferredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            else
                allocInfo.preferredFlags = 0;
            allocInfo.memoryTypeBits = 0;
            allocInfo.pool = VK_NULL_HANDLE;
            allocInfo.pUserData = nullptr;
            MAGMA_ASSERT((*priority >= 0.f) && (*priority <= 1.f));
            allocInfo.priority = *priority;
            allocInfos.push_back(allocInfo);
        });
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

DeviceMemoryBlock DeviceMemoryAllocator::realloc(DeviceMemoryBlock /* memory */, VkDeviceSize /* size */)
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

VkResult DeviceMemoryAllocator::bindMemory(DeviceMemoryBlock memory, VkDeviceSize offset,
    NonDispatchableHandle object, VkObjectType objectType) const noexcept
{
    VmaAllocation allocation = reinterpret_cast<VmaAllocation>(memory);
    switch (objectType)
    {
    case VK_OBJECT_TYPE_BUFFER:
        return vmaBindBufferMemory2(allocator, allocation, offset, core::reinterpret<VkBuffer>(object), nullptr);
    case VK_OBJECT_TYPE_IMAGE:
        return vmaBindImageMemory2(allocator, allocation, offset, core::reinterpret<VkImage>(object), nullptr);
    default:
        return VK_ERROR_FORMAT_NOT_SUPPORTED;
    }
}

MemoryBlockInfo DeviceMemoryAllocator::getMemoryBlockInfo(DeviceMemoryBlock allocation) const noexcept
{
    VmaAllocationInfo allocInfo = {};
    vmaGetAllocationInfo(allocator, reinterpret_cast<VmaAllocation>(allocation), &allocInfo);
    MemoryBlockInfo memoryInfo;
    memoryInfo.deviceMemory = allocInfo.deviceMemory;
    memoryInfo.offset = allocInfo.offset;
    memoryInfo.size = allocInfo.size;
    return memoryInfo;
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

VkResult DeviceMemoryAllocator::beginCpuDefragmentation(const std::list<std::shared_ptr<Resource>>& resources, bool incremental,
    DefragmentationStats* stats /* nullptr */)
{
    std::vector<VmaAllocation> allocations = gatherSuballocations(resources);
    VmaDefragmentationInfo2 cpuDefragInfo;
    cpuDefragInfo.flags = incremental ? VMA_DEFRAGMENTATION_FLAG_INCREMENTAL : 0;
    cpuDefragInfo.allocationCount = MAGMA_COUNT(allocations);
    cpuDefragInfo.pAllocations = allocations.data();
    cpuDefragInfo.pAllocationsChanged = allocationsChanged.data();
    cpuDefragInfo.poolCount = 0;
    cpuDefragInfo.pPools = nullptr;
    cpuDefragInfo.maxCpuBytesToMove = VK_WHOLE_SIZE;
    cpuDefragInfo.maxCpuAllocationsToMove = std::numeric_limits<uint32_t>::max();
    cpuDefragInfo.maxGpuBytesToMove = 0;
    cpuDefragInfo.maxGpuAllocationsToMove = 0;
    cpuDefragInfo.commandBuffer = VK_NULL_HANDLE;
    return vmaDefragmentationBegin(allocator, &cpuDefragInfo, reinterpret_cast<VmaDefragmentationStats*>(stats), &defragmentationContext);
}

VkResult DeviceMemoryAllocator::beginGpuDefragmentation(std::shared_ptr<CommandBuffer> cmdBuffer,
    const std::list<std::shared_ptr<Resource>>& resources, bool incremental,
    DefragmentationStats* stats /* nullptr */)
{
    std::vector<VmaAllocation> allocations = gatherSuballocations(resources);
    VmaDefragmentationInfo2 gpuDefragInfo;
    gpuDefragInfo.flags = incremental ? VMA_DEFRAGMENTATION_FLAG_INCREMENTAL : 0;
    gpuDefragInfo.allocationCount = MAGMA_COUNT(allocations);
    gpuDefragInfo.pAllocations = allocations.data();
    gpuDefragInfo.pAllocationsChanged = allocationsChanged.data();
    gpuDefragInfo.poolCount = 0;
    gpuDefragInfo.pPools = nullptr;
    gpuDefragInfo.maxCpuBytesToMove = 0;
    gpuDefragInfo.maxCpuAllocationsToMove = 0;
    gpuDefragInfo.maxGpuBytesToMove = VK_WHOLE_SIZE;
    gpuDefragInfo.maxGpuAllocationsToMove = std::numeric_limits<uint32_t>::max();
    gpuDefragInfo.commandBuffer = *cmdBuffer;
    return vmaDefragmentationBegin(allocator, &gpuDefragInfo, reinterpret_cast<VmaDefragmentationStats*>(stats), &defragmentationContext);
}

VkResult DeviceMemoryAllocator::endDefragmentation()
{   // https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/defragmentation.html
    const VkResult result = vmaDefragmentationEnd(allocator, defragmentationContext);
    core::forEach(allocationsChanged, defragmentationResources,
        [](auto& changed, auto& defragmentedResource)
        {
            if (*changed)
            {
                (*defragmentedResource)->getMemory()->onDefragment();
                (*defragmentedResource)->onDefragment();
            }
        });
    allocationsChanged.clear();
    allocationsChanged.shrink_to_fit();
    defragmentationResources.clear();
    defragmentationResources.shrink_to_fit();
    return result;
}

VkResult DeviceMemoryAllocator::map(DeviceMemoryBlock allocation, VkDeviceSize offset, void **data) noexcept
{
    MAGMA_ASSERT(data);
    *data = nullptr;
    const VkResult result = vmaMapMemory(allocator, reinterpret_cast<VmaAllocation>(allocation), data);
    if (VK_SUCCESS == result)
    {   // When succeeded, *ppData contains pointer to first byte of this memory,
        // so pointer should be offseted manually using <offset> parameter.
        *((uint8_t**)data) += offset;
    }
    return result;
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

std::vector<VmaAllocation> DeviceMemoryAllocator::gatherSuballocations(const std::list<std::shared_ptr<Resource>>& objects)
{
    std::vector<VmaAllocation> allocations;
    allocations.reserve(objects.size());
    defragmentationResources.clear();
    defragmentationResources.reserve(objects.size());
    for (auto& resource : objects)
    {
        std::shared_ptr<IDeviceMemory> deviceMemory = resource->getMemory();
        if (deviceMemory)
        {
            std::shared_ptr<ManagedDeviceMemory> managedDeviceMemory = std::dynamic_pointer_cast<ManagedDeviceMemory>(deviceMemory);
            if (managedDeviceMemory)
            {
                DeviceMemoryBlock suballocation = managedDeviceMemory->getAllocation();
                if (suballocation)
                {
                    allocations.push_back(reinterpret_cast<VmaAllocation>(suballocation));
                    defragmentationResources.push_back(resource);
                }
            }
        }
    }
    allocationsChanged.clear();
    allocationsChanged.resize(allocations.size(), VK_FALSE);
    return allocations;
}

int DeviceMemoryAllocator::chooseMemoryUsage(VkMemoryPropertyFlags flags) noexcept
{
    if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {
        if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            return VMA_MEMORY_USAGE_CPU_TO_GPU; // Memory that is both mappable on host and preferably fast to access by GPU
        else // Memory will be used on device only, so fast access from the device is preferred
            return VMA_MEMORY_USAGE_GPU_ONLY;
    }
    else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT && !(flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT))
            return VMA_MEMORY_USAGE_CPU_ONLY; // Staging copy of resources used as transfer source
        else if (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
            return VMA_MEMORY_USAGE_GPU_TO_CPU; // Memory mappable on host and cached, can be used for readback from GPU
        return VMA_MEMORY_USAGE_UNKNOWN; // Unknown usage scenario
    }
    else if (flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
    {   // Exists mostly on mobile platforms, always created as dedicated allocation
        return VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
    }
    // VK_MEMORY_PROPERTY_PROTECTED_BIT
    // VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD
    // VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD
    return VMA_MEMORY_USAGE_UNKNOWN;
}
} // namespace magma
