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
#include <vector>
#include "handle.h"

namespace magma
{
    class Device;
    class DeviceMemory;
    class Buffer;
    class CommandBuffer;

    class Image : public NonDispatchable<VkImage>
    {
    public:
        ~Image();
        VkImageType getType() const { return imageType; }
        VkFormat getFormat() const { return format; }
        VkImageLayout getLayout() const { return layout; }
        const VkExtent3D& getExtent() const { return extent; }
        VkExtent2D getExtent2D() const { return VkExtent2D{extent.width, extent.height}; }
        uint32_t getMipLevels() const { return mipLevels; }
        uint32_t getArrayLayers() const { return arrayLayers; }
        uint32_t getSamples() const { return samples; }
        void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0);
        std::shared_ptr<DeviceMemory> getMemory() const { return memory; }

    protected:
        Image(std::shared_ptr<const Device> device, 
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t samples,
            VkImageUsageFlags usage,
            VkImageCreateFlags flags = 0);
        Image(std::shared_ptr<const Device> device, 
            VkImage image,
            VkImageType imageType, 
            VkFormat format);
        VkDeviceSize getCopyRegions(const std::vector<VkExtent2D>& mipExtents,
            const std::vector<VkDeviceSize>& mipSizes,
            std::vector<VkBufferImageCopy>& copyRegions) const;
        void copyFromBuffer(std::shared_ptr<Buffer> buffer,
            const std::vector<VkBufferImageCopy>& copyRegions,
            std::shared_ptr<CommandBuffer> cmdBuffer);

    protected:
        VkImageType imageType;
        VkFormat format;
        VkImageLayout layout;
        VkExtent3D extent;
        uint32_t mipLevels;
        uint32_t arrayLayers;
        uint32_t samples;
        VkImageCreateFlags flags;
        std::shared_ptr<DeviceMemory> memory;
        friend class ImageView;
    };
} // namespace magma
