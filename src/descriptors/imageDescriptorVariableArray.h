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
#include "descriptorSetLayoutBinding.h"
#include "../objects/sampler.h"
#include "../objects/image.h"
#include "../objects/imageView.h"

namespace magma
{
    class ImageView;

    namespace descriptor
    {
        /* Base class of variable count of image/sampler descriptors. */

        class ImageDescriptorVariableArray : public DescriptorSetLayoutBinding
        {
        public:
            uint32_t getSize() const noexcept { return core::countof(descriptors); }
            void remove(uint32_t index);
            void clear();
            bool resourceBinded() const noexcept override { return true; }
            void write(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;

        protected:
            ImageDescriptorVariableArray(VkDescriptorType descriptorType,
                uint32_t binding) noexcept;
            uint32_t insert(const VkDescriptorImageInfo& imageInfo);

            std::vector<VkDescriptorImageInfo> descriptors;
            std::set<uint32_t> invalidDescriptorIndices;
        };

        /* A combined image sampler is a single descriptor type associated
           with both a sampler and an image resource, combining both
           a sampler and sampled image descriptor into a single descriptor. */

        class CombinedImageSamplerVariableArray : public ImageDescriptorVariableArray
        {
        public:
            CombinedImageSamplerVariableArray(uint32_t binding) noexcept:
                ImageDescriptorVariableArray(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding) {}
            uint32_t add(lent_ptr<const ImageView> imageView,
                lent_ptr<const magma::Sampler> sampler);
        };

        /* A sampled image is a descriptor type associated with
           an image resource via an image view that sampling
           operations can be performed on. */

        class SampledImageVariableArray : public ImageDescriptorVariableArray
        {
        public:
            SampledImageVariableArray(uint32_t binding) noexcept:
                ImageDescriptorVariableArray(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, binding) {}
            uint32_t add(lent_ptr<const ImageView> imageView);
        };
    } // namespace descriptor
} // namespace magma

#include "imageDescriptorVariableArray.inl"
