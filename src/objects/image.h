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
#pragma once
#include "resource.h"
#include "typedefs.h"

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

    class Image : public NonDispatchableResource<Image, VkImage>
    {
    public:
        struct Mip;
        struct MipData;
        struct Descriptor;
        struct CopyLayout;

    public:
        ~Image();
        VkImageCreateFlags getFlags() const noexcept { return flags; }
        VkImageType getType() const noexcept { return imageType; }
        VkFormat getFormat() const noexcept { return format; }
        VkImageLayout getLayout() const noexcept { return layout; }
        void setLayout(VkImageLayout layout_) noexcept { layout = layout_; }
        uint32_t getWidth() const noexcept { return extent.width; }
        uint32_t getHeight() const noexcept { return extent.height; }
        uint32_t getDepth() const noexcept { return extent.depth; }
        uint32_t getMipLevels() const noexcept { return mipLevels; }
        uint32_t getArrayLayers() const noexcept { return arrayLayers; }
        uint32_t getSamples() const noexcept { return samples; }
        VkImageUsageFlags getUsage() const noexcept { return usage; }
        VkExtent3D getExtent() const noexcept { return extent; }
        VkExtent3D calculateMipExtent(uint32_t level) const noexcept;
        const std::vector<VkFormat>& getViewFormats() const noexcept { return viewFormats; }
        VkSubresourceLayout getSubresourceLayout(uint32_t mipLevel,
            uint32_t arrayLayer = 0) const noexcept;
        VkImageSubresourceLayers getSubresourceLayers(uint32_t mipLevel,
            uint32_t arrayLayer = 0) const noexcept;
        VkImageSubresourceRange getSubresourceRange(uint32_t baseMipLevel,
            uint32_t baseArrayLayer = 0) const noexcept;
        VkMemoryRequirements getMemoryRequirements() const noexcept;
        std::vector<VkSparseImageMemoryRequirements> getSparseMemoryRequirements() const;
    #ifdef VK_KHR_get_memory_requirements2
        VkMemoryRequirements getMemoryRequirements2(void *memoryRequirements) const;
        std::vector<VkSparseImageMemoryRequirements2KHR> getSparseMemoryRequirements2(void *memoryRequirements) const;
    #endif // VK_KHR_get_memory_requirements2
        Class getResourceClass() const noexcept override final { return Class::Image; }
        void bindMemory(std::shared_ptr<IDeviceMemory> memory,
            VkDeviceSize offset = 0) override;
    #ifdef VK_KHR_device_group
        void bindMemoryDeviceGroup(std::shared_ptr<IDeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) override;
    #endif // VK_KHR_device_group
        void onDefragment() override;
        VkImageLayout layoutTransition(VkImageLayout newLayout,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            VkPipelineStageFlags shaderStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT) noexcept;
        VkImageLayout layoutTransitionMipLayer(VkImageLayout newLayout,
            uint32_t baseMipLevel,
            uint32_t baseArrayLayer,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            VkPipelineStageFlags shaderStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT) noexcept;
        void copyMip(std::shared_ptr<CommandBuffer> cmdBuffer,
            uint32_t mipLevel,
            uint32_t arrayLayer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
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
            const Descriptor& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);
        Image(std::shared_ptr<Device> device,
            VkImage handle,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent);
        VkDeviceSize setupMipmap(std::vector<Mip>& dstMips,
            const std::vector<MipData>& srcMips) const;
        void copyMipmap(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            const std::vector<Mip>& mipMaps,
            const CopyLayout& bufferLayout,
            VkPipelineStageFlags shaderStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
        VkExtent3D virtualMipExtent(uint32_t mipLevel) const noexcept;
        static VkSampleCountFlagBits getSampleCountBit(uint32_t samples) noexcept;
        static VkFormat checkFormatFeature(std::shared_ptr<Device> device,
            VkFormat format,
            VkFormatFeatureFlags requiredFeature);

    protected:
        const VkImageCreateFlags flags;
        const VkImageType imageType;
        const VkFormat format;
        VkImageLayout layout;
        const VkExtent3D extent;
        const uint32_t mipLevels;
        const uint32_t arrayLayers;
        const uint32_t samples;
        const VkImageTiling tiling;
        const VkImageUsageFlags usage;
        std::vector<VkFormat> viewFormats;
    };

    struct Image::Mip
    {
        VkExtent3D extent = {0, 0, 0};
        VkDeviceSize bufferOffset = 0ull;
    };

    struct Image::MipData
    {
        VkExtent3D extent = {0, 0, 0};
        VkDeviceSize size = 0ull;
        void *texels = nullptr;
    };

    struct Image::Descriptor
    {
        VkImageCreateFlags flags;
        bool lazy = false;
        // VK_KHR_image_format_list
        std::vector<VkFormat> viewFormats;
        // VK_KHR_device_group
        uint32_t deviceMask = 0;
        // VK_EXT_memory_priority
        float memoryPriority = MAGMA_MEMORY_PRIORITY_DEFAULT;
    };

    struct Image::CopyLayout
    {
        VkDeviceSize offset = 0;
        uint32_t rowLength = 0;
        uint32_t imageHeight = 0;
    };
} // namespace magma
