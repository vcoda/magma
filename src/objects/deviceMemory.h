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
#pragma once
#include "baseDeviceMemory.h"

namespace magma
{
#ifdef VK_ANDROID_external_memory_android_hardware_buffer
    class AndroidHardwareBuffer;
#endif

    /* The maximum number of valid memory allocations that can
       exist simultaneously within a VkDevice may be restricted
       by implementation- or platform-dependent limits.
       The maxMemoryAllocationCount feature describes the number
       of allocations that can exist simultaneously before
       encountering these internal limits. */

    class DeviceMemory : public BaseDeviceMemory
    {
    public:
        explicit DeviceMemory(std::shared_ptr<Device> device,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
    #ifdef VK_ANDROID_external_memory_android_hardware_buffer
        explicit DeviceMemory(std::shared_ptr<Device> device,
            lent_ptr<AndroidHardwareBuffer> hardwareBuffer,
            VkMemoryPropertyFlags flags,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
    #endif // VK_ANDROID_external_memory_android_hardware_buffer
        ~DeviceMemory();
        static uint32_t getAllocationCount() noexcept { return allocationCount; }
        VkDeviceSize getSuballocationOffset() const noexcept override { return 0ull; }
        float getPriority() const noexcept override { return priority; }
        void setPriority(float priority) noexcept override;
    #ifdef VK_ANDROID_external_memory_android_hardware_buffer
        AHardwareBuffer* getHardwareBuffer() const noexcept;
    #endif
        bool managed() const noexcept override { return false; }
        void realloc(NonDispatchableHandle object,
            const VkMemoryRequirements& memoryRequirements,
            const StructureChain& extendedInfo = StructureChain()) override;
        void bind(NonDispatchableHandle object,
            VkObjectType objectType,
            VkDeviceSize offset = 0) override;
    #ifdef VK_KHR_device_group
        void bindDeviceGroup(NonDispatchableHandle object,
            VkObjectType objectType,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) override;
    #endif // VK_KHR_device_group
        void *map(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkMemoryMapFlags flags = 0,
            bool persistently = false) noexcept override;
        void unmap() noexcept override;
        bool flushMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept override;
        bool invalidateMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept override;
        VkDeviceSize getCommitment() noexcept override;
        void onDefragment() noexcept override {}

    private:
        static std::atomic<uint32_t> allocationCount;
    };
} // namespace magma
