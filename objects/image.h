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
    class CommandBuffer;

    class Image : public NonDispatchable<VkImage>
    {
    public:
        ~Image();
        virtual VkImageType getType() const = 0;
        VkFormat getFormat() const { return format; }
        VkImageLayout getLayout() const { return layout; }
        const VkExtent3D& getExtent() const { return extent; }
        uint32_t getMipLevels() const { return mipLevels; }
        void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0);
        std::shared_ptr<DeviceMemory> getMemory() const { return memory; }

    protected:
        Image(std::shared_ptr<const Device> device, 
            VkImageType imageType, 
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            VkImageUsageFlags usage);
        Image(std::shared_ptr<const Device> device, VkFormat format);

    protected:
        VkFormat format;
        VkImageLayout layout;
        VkExtent3D extent;
        uint32_t mipLevels;
        std::shared_ptr<DeviceMemory> memory;
    };

    class Image1D : public Image
    {
    public:
        Image1D(std::shared_ptr<const Device> device,
            VkFormat format,
            uint32_t width,
            uint32_t mipLevels,
            VkImageUsageFlags usage);
        VkImageType getType() const override { return VK_IMAGE_TYPE_1D; }
    };

    class Image2D : public Image
    {
    public:
        Image2D(std::shared_ptr<const Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            VkImageUsageFlags usage);
        Image2D(std::shared_ptr<const Device> device, 
            VkFormat format, 
            const std::vector<const void *>& mipLevelData,
            const std::vector<VkExtent2D>& mipLevelExtents,
            const std::vector<VkDeviceSize>& mipLevelSizes,
            std::shared_ptr<CommandBuffer> cmdBuffer);
        VkImageType getType() const override { return VK_IMAGE_TYPE_2D; }
    };

    class Image3D : public Image
    {
    public:
        Image3D(std::shared_ptr<const Device> device,
            VkFormat format,
            const VkExtent3D& extent,
            VkImageUsageFlags usage);
        VkImageType getType() const override { return VK_IMAGE_TYPE_3D; }
    };

    class SwapchainImage : public Image
    {
        friend class Swapchain;
        SwapchainImage(std::shared_ptr<const Device> device,
            VkImage image,
            VkFormat format);
    public:
        ~SwapchainImage();
        VkImageType getType() const override { return VK_IMAGE_TYPE_2D; }
    };
} // namespace magma
