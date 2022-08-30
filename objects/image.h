/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    class IDeviceMemoryAllocator;

    /* Images represent multidimensional - up to 3 - arrays of data
       which can be used for various purposes (e.g. attachments, textures),
       by binding them to a graphics or compute pipeline via descriptor sets,
       or by directly specifying them as parameters to certain commands. */

    class Image : public NonDispatchableResource<Image, VkImage>
    {
    public:
        struct CopyLayout;
        typedef std::vector<VkDeviceSize> MipmapLayout;
        typedef std::vector<const void *> MipmapData;
        typedef std::vector<MipmapData> ArrayMipmapData;

    public:
        ~Image();
        VkImageCreateFlags getFlags() const noexcept { return flags; }
        VkImageType getType() const noexcept { return imageType; }
        VkFormat getFormat() const noexcept { return format; }
        VkImageLayout getLayout() const noexcept { return layout; }
        void setLayout(VkImageLayout layout) noexcept { this->layout = layout; }
        VkExtent3D getMipExtent(uint32_t level) const noexcept;
        uint32_t getMipLevels() const noexcept { return mipLevels; }
        uint32_t getArrayLayers() const noexcept { return arrayLayers; }
        uint32_t getSamples() const noexcept { return samples; }
        VkImageUsageFlags getUsage() const noexcept { return usage; }
        bool hasStorageFlag() const noexcept { return usage & VK_IMAGE_USAGE_STORAGE_BIT; }
        bool hasColorAttachmentFlag() const noexcept { return usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; }
        bool hasDepthStencilAttachmentFlag() const noexcept { return usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
        bool hasInputAttachmentFlag() const noexcept { return usage & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT; }
        const std::vector<VkFormat>& getViewFormats() const noexcept { return viewFormats; }
        VkSubresourceLayout getSubresourceLayout(uint32_t mipLevel,
            uint32_t arrayLayer = 0) const noexcept;
        VkImageSubresourceLayers getSubresourceLayers(uint32_t mipLevel,
            uint32_t arrayLayer = 0) const noexcept;
        VkMemoryRequirements getMemoryRequirements() const noexcept;
        std::vector<VkSparseImageMemoryRequirements> getSparseMemoryRequirements() const;
        virtual void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0) override;
#ifdef VK_KHR_device_group
        virtual void bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            VkDeviceSize offset = 0) override;
        void bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions,
            VkDeviceSize offset = 0);
#endif
        virtual void onDefragment() override;
        void copyMipLevel(std::shared_ptr<CommandBuffer> cmdBuffer,
            uint32_t level,
            std::shared_ptr<const Buffer> srcBuffer,
            const CopyLayout& bufferLayout,
            const VkOffset3D& imageOffset,
            VkImageLayout dstLayout,
            VkPipelineStageFlags barrierDstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);

    protected:
        explicit Image(std::shared_ptr<Device> device,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t samples,
            VkImageTiling tiling,
            VkImageUsageFlags usage,
            VkImageCreateFlags flags,
            const std::vector<VkFormat> viewFormats,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);
        explicit Image(std::shared_ptr<Device> device,
            VkImage handle,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent);
        MipmapLayout setupMipOffsets(const MipmapLayout& mipSizes,
            VkDeviceSize& bufferSize) const noexcept;
        std::vector<VkBufferImageCopy> setupCopyRegions(const MipmapLayout& mipOffsets,
            const CopyLayout& bufferLayout) const noexcept;
        void copyTransfer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const Buffer> buffer,
            const std::vector<VkBufferImageCopy>& copyRegions);
        static VkSampleCountFlagBits getSampleCountBit(uint32_t samples);

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

    struct Image::CopyLayout
    {
        VkDeviceSize offset = 0;
        uint32_t rowLength = 0;
        uint32_t imageHeight = 0;
    };
} // namespace magma
