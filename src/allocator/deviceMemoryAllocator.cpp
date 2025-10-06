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
#include "deviceMemoryAllocator.h"
#include "../objects/managedDeviceMemory.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/instance.h"
#include "../objects/commandBuffer.h"
#include "../exceptions/errorResult.h"
#include "../misc/compatibility.h"

#if defined(_MSC_VER)
    #pragma warning(push, 0)
    #pragma warning(disable: 4100) // unreferenced formal parameter
    #pragma warning(disable: 4127) // conditional expression is constant
    #pragma warning(disable: 4189) // local variable is initialized but not referenced
    #pragma warning(disable: 4324) // structure was padded due to alignment specifier
    #pragma warning(disable: 4505) // unreferenced local function has been removed
#elif defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#elif defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-variable"
    #pragma clang diagnostic ignored "-Wunused-parameter"
    #pragma clang diagnostic ignored "-Wunused-private-field"
    #pragma clang diagnostic ignored "-Wmissing-field-initializers"
    #pragma clang diagnostic ignored "-Wnullability-completeness"
    #pragma clang diagnostic ignored "-Wcast-align"
#endif // __clang__

#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VMA_KHR_MAINTENANCE4 0
#include "../third-party/VulkanMemoryAllocator/include/vk_mem_alloc.h"

#if defined(_MSC_VER)
    #pragma warning(pop)
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(__clang__)
    #pragma clang diagnostic pop
#endif // __clang__

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
    const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.flags = 0;
#if defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_dedicated_allocation)
    if (device->extensionEnabled(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME) &&
        device->extensionEnabled(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME))
    {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
    }
#endif // VK_KHR_get_memory_requirements2 && VK_KHR_dedicated_allocation
#if defined(VK_KHR_buffer_device_address)
    if (device->extensionEnabled(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME))
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
#elif defined(VK_EXT_buffer_device_address)
    if (device->extensionEnabled(VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME))
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
#endif // VK_EXT_buffer_device_address
#ifdef VK_EXT_memory_priority
    if (device->extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT;
#endif // VK_EXT_memory_priority
    allocatorInfo.physicalDevice = *physicalDevice;
    allocatorInfo.device = *device;
    allocatorInfo.preferredLargeHeapBlockSize = 0;
    allocatorInfo.pAllocationCallbacks = hostAllocator.get();
    allocatorInfo.pDeviceMemoryCallbacks = nullptr;
    allocatorInfo.pHeapSizeLimit = nullptr;
    allocatorInfo.pVulkanFunctions = nullptr;
    allocatorInfo.instance = physicalDevice->getInstance()->getHandle();
    allocatorInfo.vulkanApiVersion = physicalDevice->getInstance()->getApiVersion();
#ifdef VK_KHR_external_memory
    allocatorInfo.pTypeExternalMemoryHandleTypes = nullptr;
#endif
    const VkResult result = vmaCreateAllocator(&allocatorInfo, &allocator);
    MAGMA_HANDLE_RESULT(result, "failed to create VMA allocator");
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
    vmaDestroyAllocator(allocator);
}

DeviceMemoryBlock DeviceMemoryAllocator::allocate(VkObjectType objectType, NonDispatchableHandle object,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    const StructureChain& extendedInfo)
{
    MAGMA_UNUSED(extendedInfo);
    VmaAllocationCreateInfo allocInfo;
    allocInfo.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
    allocInfo.usage = (VmaMemoryUsage)chooseMemoryUsage(flags);
    if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    else
        allocInfo.requiredFlags = 0;
    if (VMA_MEMORY_USAGE_CPU_TO_GPU == allocInfo.usage) // Memory that is both mappable on host and preferably fast to access by GPU
        allocInfo.preferredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    else
        allocInfo.preferredFlags = 0;
    allocInfo.memoryTypeBits = 0;
    allocInfo.pool = VK_NULL_HANDLE;
    allocInfo.pUserData = nullptr;
    allocInfo.priority = MemoryPriorityDefault;
#ifdef VK_EXT_memory_priority
    if (device->extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
        const VkMemoryPriorityAllocateInfoEXT *memoryPriorityAllocateInfo = extendedInfo.findNode<VkMemoryPriorityAllocateInfoEXT>();
        if (memoryPriorityAllocateInfo)
            allocInfo.priority = std::clamp(memoryPriorityAllocateInfo->priority, 0.f, 1.f);
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
    MAGMA_HANDLE_RESULT(result, "failed to allocate memory");
    return reinterpret_cast<DeviceMemoryBlock>(allocation);
}

std::vector<DeviceMemoryBlock> DeviceMemoryAllocator::allocPages(const std::vector<VkMemoryRequirements>& memoryRequirements,
    const std::vector<VkMemoryPropertyFlags>& memoryFlags, const std::vector<float>& priorities)
{
    std::vector<VmaAllocationCreateInfo> allocInfos;
    allocInfos.reserve(memoryFlags.size());
    std::vector<float> defaultPriorities;
    if (priorities.empty())
        defaultPriorities.resize(memoryFlags.size(), MemoryPriorityDefault);
    core::foreach(memoryFlags, priorities.empty() ? defaultPriorities : priorities,
        [&allocInfos](auto& flags, auto& priority)
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
            if (VMA_MEMORY_USAGE_CPU_TO_GPU == allocInfo.usage) // Memory that is both mappable on host and preferably fast to access by GPU
                allocInfo.preferredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            else
                allocInfo.preferredFlags = 0;
            allocInfo.memoryTypeBits = 0;
            allocInfo.pool = VK_NULL_HANDLE;
            allocInfo.pUserData = nullptr;
            MAGMA_ASSERT((priority >= MemoryPriorityLowest) && (priority <= MemoryPriorityHighest));
            allocInfo.priority = priority;
            allocInfos.push_back(allocInfo);
        });
    std::vector<VmaAllocation> allocations(core::countof(allocInfos));
    const VkResult result = vmaAllocateMemoryPages(allocator, memoryRequirements.data(), allocInfos.data(),
        allocations.size(), allocations.data(), nullptr);
    MAGMA_HANDLE_RESULT(result, "failed to allocate memory pages");
    std::vector<DeviceMemoryBlock> memoryPages;
    memoryPages.reserve(allocations.size());
    for (auto& allocation: allocations)
        memoryPages.push_back(reinterpret_cast<DeviceMemoryBlock>(allocation));
    return memoryPages;
}

DeviceMemoryBlock DeviceMemoryAllocator::realloc(DeviceMemoryBlock /* memory */, VkDeviceSize /* size */)
{
    MAGMA_HANDLE_RESULT(VK_ERROR_FEATURE_NOT_PRESENT, "vmaResizeAllocation() deprecated");
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
        vmaFreeMemoryPages(allocator, core::countof(allocations), reinterpret_cast<VmaAllocation *>(allocations.data()));
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
    vmaGetHeapBudgets(allocator, budgets);
    const VkPhysicalDeviceMemoryProperties& memoryProperties = device->getPhysicalDevice()->getMemoryProperties();
    std::vector<MemoryBudget> heapBudgets(memoryProperties.memoryHeapCount);
    memcpy(heapBudgets.data(), budgets, sizeof(VmaBudget) * memoryProperties.memoryHeapCount);
    return heapBudgets;
}

VkResult DeviceMemoryAllocator::checkCorruption(uint32_t memoryTypeBits) noexcept
{
    return vmaCheckCorruption(allocator, memoryTypeBits);
}

VkResult DeviceMemoryAllocator::beginDefragmentation(VkFlags flags) noexcept
{
    VmaDefragmentationInfo defragInfo = {};
    defragInfo.flags = flags;
    defragInfo.pool = VK_NULL_HANDLE;
    defragInfo.maxBytesPerPass = 0;
    defragInfo.maxAllocationsPerPass = 0;
    defragInfo.pfnBreakCallback = nullptr;
    defragInfo.pBreakCallbackUserData = nullptr;
    return vmaBeginDefragmentation(allocator, &defragInfo, &defragmentationContext);
}

VkResult DeviceMemoryAllocator::beginDefragmentationPass()
{
    if (!passInfo)
        passInfo = std::make_unique<VmaDefragmentationPassMoveInfo>();
    const VkResult result = vmaBeginDefragmentationPass(allocator, defragmentationContext, passInfo.get());
    if (VK_SUCCESS == result)
    {
        for (uint32_t i = 0; i < passInfo->moveCount; ++i)
        {
            const VmaDefragmentationMove& move = passInfo->pMoves[i];
            VmaAllocationInfo allocInfo;
            vmaGetAllocationInfo(allocator, move.srcAllocation, &allocInfo);
            // TODO: https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/defragmentation.html
        }
    }
    return result;
}

VkResult DeviceMemoryAllocator::endDefragmentationPass() noexcept
{
    return vmaEndDefragmentationPass(allocator, defragmentationContext, passInfo.get());
}

void DeviceMemoryAllocator::endDefragmentation(DefragmentationStats* stats /* nullptr */) noexcept
{
    vmaEndDefragmentation(allocator, defragmentationContext, reinterpret_cast<VmaDefragmentationStats *>(stats));
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

std::vector<VmaAllocation> DeviceMemoryAllocator::gatherSuballocations(const std::list<std::shared_ptr<IResource>>& objects)
{
    std::vector<VmaAllocation> allocations;
    allocations.reserve(objects.size());
    defragmentationResources.clear();
    defragmentationResources.reserve(objects.size());
    for (auto& resource: objects)
    {
        const ManagedDeviceMemory *managedDeviceMemory = dynamic_cast<const ManagedDeviceMemory *>(resource->getMemory().get());
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
