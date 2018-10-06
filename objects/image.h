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
#include "../typedefs.h"

namespace magma
{
    class DeviceMemory;
    class Buffer;
    class CommandBuffer;

    /* Images represent multidimensional - up to 3 - arrays of data
       which can be used for various purposes (e.g. attachments, textures),
       by binding them to a graphics or compute pipeline via descriptor sets,
       or by directly specifying them as parameters to certain commands. */

    class Image : public NonDispatchable<VkImage>
    {
    public:
        ~Image();
        VkImageType getType() const noexcept { return imageType; }
        VkFormat getFormat() const noexcept { return format; }
        VkImageLayout getLayout() const noexcept { return layout; }
        const VkExtent3D& getExtent() const noexcept { return extent; }
        VkExtent2D getExtent2D() const noexcept { return VkExtent2D{extent.width, extent.height}; }
        uint32_t getMipLevels() const noexcept { return mipLevels; }
        uint32_t getArrayLayers() const noexcept { return arrayLayers; }
        uint32_t getSamples() const noexcept { return samples; }
        void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0);
        void bindMemoryDeviceGroup(const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions,
            std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0);
        std::shared_ptr<DeviceMemory> getMemory() const noexcept { return memory; }

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
            std::shared_ptr<IAllocator> allocator);
        explicit Image(std::shared_ptr<Device> device,
            VkImage handle,
            VkImageType imageType,
            VkFormat format,
            const VkExtent3D& extent);
        std::vector<VkBufferImageCopy> getCopyRegions(const std::vector<VkExtent2D>& mipExtents,
            const std::vector<VkDeviceSize>& mipSizes,
            VkDeviceSize *size) const noexcept;
        void copyFromBuffer(std::shared_ptr<Buffer> buffer,
            const std::vector<VkBufferImageCopy>& copyRegions,
            std::shared_ptr<CommandBuffer> copyCmdBuffer);

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
