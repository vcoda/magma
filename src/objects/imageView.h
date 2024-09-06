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
#include "model/nondispatchable.h"

namespace magma
{
    class Image;
    class Sampler;

    /* Image objects are not directly accessed by pipeline shaders
       for reading or writing image data. Instead, image views
       representing contiguous ranges of the image subresources
       and containing additional metadata are used for that purpose.
       Views must be created on images of compatible types,
       and must represent a valid subset of image subresources. */

    class ImageView : public NonDispatchable<VkImageView>
    {
    public:
        explicit ImageView(std::shared_ptr<Image> image,
            const VkComponentMapping& swizzle = {
                // https://github.com/SaschaWillems/Vulkan/issues/160
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY},
            VkImageViewCreateFlags flags = 0,
            VkImageUsageFlags usage = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit ImageView(std::shared_ptr<Image> image,
            uint32_t baseMipLevel,
            uint32_t levelCount = VK_REMAINING_MIP_LEVELS,
            uint32_t baseArrayLayer = 0,
            uint32_t layerCount = VK_REMAINING_ARRAY_LAYERS,
            const VkComponentMapping& swizzle = {
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY},
            VkImageViewCreateFlags flags = 0,
            VkImageUsageFlags usage = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~ImageView();
        const std::shared_ptr<Image>& getImage() const noexcept { return image; }
        VkImageViewCreateFlags getFlags() const noexcept { return flags; }
    #ifdef VK_KHR_maintenance2
        VkImageUsageFlags getUsage() const noexcept { return usage; }
    #endif
        uint32_t getBaseMipLevel() const noexcept { return baseMipLevel; }
        uint32_t getMipLevelCount() const noexcept;
        uint32_t getBaseArrayLayer() const noexcept { return baseArrayLayer; }
        uint32_t getArrayLayerCount() const noexcept;
        VkExtent3D getExtent() const noexcept;
        VkDescriptorImageInfo getDescriptor(std::shared_ptr<const Sampler> sampler) const noexcept;

    protected:
        ImageView(std::shared_ptr<Image> image,
            uint32_t baseMipLevel,
            uint32_t levelCount,
            uint32_t baseArrayLayer,
            uint32_t layerCount,
            VkImageViewCreateFlags flags);
        static VkImageViewType imageToViewType(VkImageType imageType,
            uint32_t arrayLayers,
            VkImageCreateFlags flags) noexcept;

        std::shared_ptr<Image> image;
        const VkImageViewCreateFlags flags;
    #ifdef VK_KHR_maintenance2
        const VkImageUsageFlags usage;
    #endif
        const uint32_t baseMipLevel;
        const uint32_t levelCount;
        const uint32_t baseArrayLayer;
        const uint32_t layerCount;
    };
} // namespace magma
