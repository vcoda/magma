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
#include "model/nondispatchable.h"
#include "resource.h"
#include "ideviceMemory.h"

namespace magma
{
    class SrcTransferBuffer;
    class CommandBuffer;

    /* Buffers represent linear arrays of data which are used
       for various purposes by binding them to a graphics or
       compute pipeline via descriptor sets or via certain
       commands, or by directly specifying them as parameters
       to certain commands. */

    class Buffer : public NonDispatchable<VkBuffer>,
        public Resource
    {
    public:
        struct Initializer;
        ~Buffer();
        VkBufferCreateFlags getFlags() const noexcept { return flags; }
        VkBufferUsageFlags getUsage() const noexcept { return usage; }
        Family getFamily() const noexcept override final { return Family::Buffer; }
        VkMemoryRequirements getMemoryRequirements() const noexcept override;
    #ifdef VK_KHR_get_memory_requirements2
        VkMemoryRequirements getMemoryRequirements2(void *memoryRequirements) const override;
    #endif
        VkSparseMemoryBind getSparseMemoryBind(VkDeviceSize bufferOffset,
            VkDeviceSize size,
            VkDeviceSize memoryOffset) const noexcept;
        virtual VkDescriptorBufferInfo getDescriptor() const noexcept;
        void realloc(VkDeviceSize newSize);
        void bindMemory(std::unique_ptr<IDeviceMemory> memory,
            VkDeviceSize offset = 0) override;
    #ifdef VK_KHR_device_group
        void bindMemoryDeviceGroup(std::unique_ptr<IDeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) override;
    #endif // VK_KHR_device_group
    #if defined(VK_KHR_buffer_device_address) || defined(VK_EXT_buffer_device_address)
        VkDeviceAddress getDeviceAddress() const noexcept override;
    #endif
        void onDefragment() override;
        VkDeviceSize hostCopy(const void *srcBuffer,
            VkDeviceSize srcBufferSize,
            VkDeviceSize srcOffset = 0,
            VkDeviceSize dstOffset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE,
            CopyMemoryFn copyMem = nullptr) noexcept;
        VkDeviceSize transferCopy(lent_ptr<CommandBuffer> cmdBuffer,
            lent_ptr<const Buffer> srcBuffer,
            VkDeviceSize srcOffset = 0,
            VkDeviceSize dstOffset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept;
        VkDeviceSize stagingCopy(lent_ptr<CommandBuffer> cmdBuffer,
            const void *srcBuffer,
            VkDeviceSize srcBufferSize,
            VkDeviceSize srcOffset = 0,
            VkDeviceSize dstOffset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE,
            std::shared_ptr<Allocator> allocator = nullptr,
            CopyMemoryFn copyMem = nullptr);

    protected:
        Buffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkBufferCreateFlags flags,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memoryFlags,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);

        const VkBufferCreateFlags flags;
        const VkBufferUsageFlags usage;
    };

    struct Buffer::Initializer
    {
        VkBufferCreateFlags flags;
        VkBool32 srcTransfer: 1;
        VkBool32 storage: 1;
        VkBool32 deviceAddress: 1; // VK_KHR_buffer_device_address
        VkBool32 deviceAddressCaptureReplay: 1;
        VkBool32 lazilyAllocated: 1;
        VkBool32 aliasingMemory: 1;
        uint32_t deviceMask; // VK_KHR_device_group
        float memoryPriority; // VK_EXT_memory_priority
        Initializer() noexcept:
            flags(0),
            srcTransfer(VK_FALSE),
            storage(VK_FALSE),
            deviceAddress(VK_FALSE),
            deviceAddressCaptureReplay(VK_FALSE),
            lazilyAllocated(VK_FALSE),
            aliasingMemory(VK_FALSE),
            deviceMask(0),
            memoryPriority(MemoryPriorityDefault)
        {}
    };
} // namespace magma
