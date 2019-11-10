/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../helpers/typedefs.h"

namespace magma
{
    class Buffer;
    class CommandBuffer;
    typedef std::vector<VkDeviceSize> ImageMipmapLayout;

    /* Images represent multidimensional - up to 3 - arrays of data
       which can be used for various purposes (e.g. attachments, textures),
       by binding them to a graphics or compute pipeline via descriptor sets,
       or by directly specifying them as parameters to certain commands. */

    class Image : public NonDispatchableResource<Image, VkImage>
    {
    public:
        ~Image();
        VkImageType getType() const noexcept { return imageType; }
        VkFormat getFormat() const noexcept { return format; }
        VkImageLayout getLayout() const noexcept { return layout; }
        VkExtent3D getMipExtent(uint32_t level) const;
        uint32_t getMipLevels() const noexcept { return mipLevels; }
        uint32_t getArrayLayers() const noexcept { return arrayLayers; }
        uint32_t getSamples() const noexcept { return samples; }
        VkImageUsageFlags getUsage() const noexcept { return usage; }
        VkMemoryRequirements getMemoryRequirements() const noexcept;
        std::vector<VkSparseImageMemoryRequirements> getSparseMemoryRequirements() const;
        void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0);
        void bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions,
            VkDeviceSize offset = 0);
        void copyMipLevel(uint32_t level,
            std::shared_ptr<Buffer> buffer,
            VkDeviceSize bufferOffset,
            const VkOffset3D& imageOffset,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            VkPipelineStageFlags barrierDstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
            bool flush = true);

    protected:
        explicit Image(std::shared_ptr<Device> device,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t samples,
            VkImageUsageFlags usage,
            VkImageCreateFlags flags,
            const Sharing& sharing,
            std::shared_ptr<IAllocator> allocator);
        explicit Image(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t samples,
            VkImageUsageFlags usage,
            VkImageCreateFlags flags,
            const Sharing& sharing,
            std::shared_ptr<IAllocator> allocator);
        explicit Image(std::shared_ptr<Device> device,
            VkImage handle,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent);
        ImageMipmapLayout buildMipOffsets(const ImageMipmapLayout& mipSizes,
            VkDeviceSize& bufferSize) const noexcept;
        std::vector<VkBufferImageCopy> buildCopyRegions(const ImageMipmapLayout& mipOffsets,
            VkDeviceSize bufferOffset) const noexcept;
        void copyFromBuffer(std::shared_ptr<Buffer> buffer,
            const std::vector<VkBufferImageCopy>& copyRegions,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            bool flush = true);

    protected:
        VkImageType imageType;
        VkFormat format;
        VkImageLayout layout;
        VkExtent3D extent;
        uint32_t mipLevels;
        uint32_t arrayLayers;
        uint32_t samples;
        VkImageUsageFlags usage;
        VkImageCreateFlags flags;
        friend class ImageView;
    };

    typedef std::vector<const void *> ImageMipmapData;
    typedef std::vector<ImageMipmapData> ImageArrayMipmapData;
} // namespace magma
