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
    class Buffer;
    class SrcTransferBuffer;
    class CommandBuffer;

    /* Images represent multidimensional - up to 3 - arrays
       of data which can be used for various purposes (e.g.
       attachments, textures), by binding them to a graphics
       or compute pipeline via descriptor sets, or by directly
       specifying them as parameters to certain commands. */

    class Image : public Resource,
        public NonDispatchable<VkImage>
    {
    public:
        struct Mip;
        struct CopyLayout;
        struct Initializer;

    public:
        ~Image();
        VkImageCreateFlags getFlags() const noexcept { return flags; }
        VkImageType getType() const noexcept { return imageType; }
        VkFormat getFormat() const noexcept { return format; }
        Family getFamily() const noexcept override final { return Family::Image; }
        VkImageLayout getLayout(uint32_t level) const noexcept { return mipLayouts[level]; }
        void setLayout(uint32_t level, VkImageLayout layout) noexcept { mipLayouts[level] = layout; }
        void setLayout(VkImageLayout layout) noexcept { mipLayouts.fill(layout); }
        bool hasUniformLayout() const noexcept;
        uint32_t getWidth() const noexcept { return extent.width; }
        uint32_t getHeight() const noexcept { return extent.height; }
        uint32_t getDepth() const noexcept { return extent.depth; }
        uint32_t getMipLevels() const noexcept { return mipLevels; }
        uint32_t getArrayLayers() const noexcept { return arrayLayers; }
        uint32_t getSamples() const noexcept { return samples; }
        VkImageTiling getTiling() const noexcept { return tiling; }
        VkImageUsageFlags getUsage() const noexcept { return usage; }
        VkImageAspectFlags getAspectMask() const noexcept;
        VkExtent3D getExtent() const noexcept { return extent; }
        VkExtent3D calculateMipExtent(uint32_t level) const noexcept;
        uint32_t getLevelTexelCount(uint32_t level) const noexcept;
        uint32_t getTexelCount() const noexcept;
        const std::vector<VkFormat>& getViewFormats() const noexcept { return viewFormats; }
        VkSubresourceLayout getSubresourceLayout(uint32_t mipLevel,
            uint32_t arrayLayer = 0) const noexcept;
        VkImageSubresourceLayers getSubresourceLayers(uint32_t mipLevel,
            uint32_t baseArrayLayer = 0) const noexcept;
        VkImageSubresourceRange getSubresourceRange(uint32_t baseMipLevel,
            uint32_t baseArrayLayer = 0) const noexcept;
        VkSparseImageMemoryBind getSparseMemoryBind(uint32_t mipLevel,
            uint32_t arrayLayer,
            const VkOffset3D& offset,
            const VkExtent3D& extent,
            VkDeviceSize memoryOffset) const noexcept;
        VkMemoryRequirements getMemoryRequirements() const noexcept override;
        std::vector<VkSparseImageMemoryRequirements> getSparseMemoryRequirements() const;
    #ifdef VK_KHR_get_memory_requirements2
        VkMemoryRequirements getMemoryRequirements2(void *memoryRequirements) const override;
        std::vector<VkSparseImageMemoryRequirements2KHR> getSparseMemoryRequirements2(void *memoryRequirements) const;
    #endif // VK_KHR_get_memory_requirements2
        void bindMemory(std::unique_ptr<IDeviceMemory> memory,
            VkDeviceSize offset = 0) override;
    #ifdef VK_KHR_device_group
        void bindMemoryDeviceGroup(std::unique_ptr<IDeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) override;
    #endif // VK_KHR_device_group
    #if defined(VK_KHR_buffer_device_address) || defined(VK_EXT_buffer_device_address)
        VkDeviceAddress getDeviceAddress() const noexcept override { return MAGMA_NULL; }
    #endif
        void onDefragment() override;
        VkImageLayout layoutTransition(VkImageLayout newLayout,
            lent_ptr<CommandBuffer> cmdBuffer,
            VkPipelineStageFlags shaderStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        VkImageLayout layoutTransitionBaseMipLayer(VkImageLayout newLayout,
            uint32_t baseMipLevel,
            uint32_t baseArrayLayer,
            lent_ptr<CommandBuffer> cmdBuffer,
            VkPipelineStageFlags shaderStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void copyMip(lent_ptr<CommandBuffer> cmdBuffer,
            uint32_t mipLevel,
            uint32_t arrayLayer,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            const CopyLayout& bufferLayout,
            const VkOffset3D& imageOffset = {0, 0, 0});
        void copyMipWithTransition(lent_ptr<CommandBuffer> cmdBuffer,
            uint32_t mipLevel,
            uint32_t arrayLayer,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            const CopyLayout& bufferLayout,
            const VkOffset3D& imageOffset,
            VkImageLayout dstLayout,
            VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

    protected:
        Image(std::shared_ptr<Device> device,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t samples,
            VkImageCreateFlags flags,
            VkImageUsageFlags usage,
            VkImageTiling tiling,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);
        Image(std::shared_ptr<Device> device,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t samples,
            VkImageCreateFlags flags,
            VkImageUsageFlags usage,
            VkImageTiling tiling);
        void copyMipmap(lent_ptr<CommandBuffer> cmdBuffer,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            const std::vector<Mip>& mipMap,
            const CopyLayout& bufferLayout,
            VkImageLayout dstLayout,
            VkPipelineStageFlags dstStageMask);
        bool copyMipmap(lent_ptr<CommandBuffer> cmdBuffer,
            const std::vector<Mip>& mipMap,
            std::shared_ptr<Allocator> allocator,
            CopyMemoryFn copyMem,
            VkImageLayout dstLayout,
            VkPipelineStageFlags dstStageMask);
        VkExtent3D calculateValidMipExtent(uint32_t mipLevel) const noexcept;
        VkPipelineStageFlags getSuitableDstStageMask(uint32_t queueFamilyIndex) const;
        static VkSampleCountFlagBits getSampleCountBit(uint32_t samples) noexcept;
        static VkFormat checkFormatFeature(std::shared_ptr<Device> device,
            VkFormat format,
            VkFormatFeatureFlags requiredFeature);

    protected:
        const VkImageCreateFlags flags;
        const VkImageType imageType;
        const VkFormat format;
        const VkExtent3D extent;
        const uint32_t mipLevels;
        const uint32_t arrayLayers;
        const uint32_t samples;
        const VkImageTiling tiling;
        const VkImageUsageFlags usage;
        std::array<VkImageLayout, 15> mipLayouts; // log2(16384) + 1
        std::vector<VkFormat> viewFormats;
    };

    struct Image::Mip
    {
        VkExtent3D extent;
        union {
            VkDeviceSize bufferOffset;
            VkDeviceSize size;
        };
        const void *texels;
        Mip() noexcept;
        Mip(uint32_t, uint32_t, VkDeviceSize) noexcept;
        Mip(const VkExtent2D&, VkDeviceSize) noexcept;
        Mip(const VkExtent3D&, VkDeviceSize) noexcept;
        Mip(uint32_t, uint32_t, VkDeviceSize, const void *) noexcept;
        Mip(const VkExtent2D&, VkDeviceSize, const void *) noexcept;
        Mip(const VkExtent3D&, VkDeviceSize, const void *) noexcept;
    };

    struct Image::CopyLayout
    {
        VkDeviceSize offset = 0;
        uint32_t rowLength = 0;
        uint32_t imageHeight = 0;
    };

    struct Image::Initializer
    {
        VkImageCreateFlags flags;
        VkBool32 srcTransfer: 1;
        VkBool32 storage: 1;
        VkBool32 lazilyAllocated: 1;
        VkBool32 aliasingMemory: 1;
        std::vector<VkFormat> viewFormats; // VK_KHR_image_format_list
        uint32_t deviceMask; // VK_KHR_device_group
        float memoryPriority; // VK_EXT_memory_priority
        Initializer() noexcept:
            flags(0),
            srcTransfer(VK_FALSE),
            storage(VK_FALSE),
            lazilyAllocated(VK_FALSE),
            aliasingMemory(VK_FALSE),
            deviceMask(0),
            memoryPriority(MemoryPriorityDefault)
        {}
    };
} // namespace magma

#include "image.inl"
