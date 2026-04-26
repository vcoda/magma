/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#include "descriptorPool.h"
#include "../objects/sampler.h"
#include "../objects/image.h"
#include "../objects/imageView.h"

namespace magma
{
    class ImageView;
}

namespace magma::descriptor
{
    /* Base class of variable count of image/sampler descriptors. */

    class VariableCountImageDescriptors : public magma::DescriptorSetLayoutBinding
    {
    public:
        void reserve(std::size_t capacity);
        bool erase(BindlessHandle handle);
        void clear();
        bool resourceBinded() const noexcept override { return true; }
        void write(VkDescriptorSet dstSet,
            VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;

    protected:
        VariableCountImageDescriptors(VkDescriptorType descriptorType,
            uint32_t binding) noexcept;

        DescriptorPool<VkDescriptorImageInfo> descriptors;
    };

    /* A combined image sampler is a single descriptor type associated
       with both a sampler and an image resource, combining both
       a sampler and sampled image descriptor into a single descriptor. */

    class VariableCountCombinedImageSamplers : public VariableCountImageDescriptors
    {
    public:
        VariableCountCombinedImageSamplers(uint32_t binding) noexcept:
            VariableCountImageDescriptors(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding) {}
        BindlessHandle insert(lent_ptr<const ImageView> imageView,
            lent_ptr<const magma::Sampler> sampler);
    };

    /* A sampled image is a descriptor type associated with
       an image resource via an image view that sampling
       operations can be performed on. */

    class VariableCountSampledImages : public VariableCountImageDescriptors
    {
    public:
        VariableCountSampledImages(uint32_t binding) noexcept:
            VariableCountImageDescriptors(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, binding) {}
        BindlessHandle insert(lent_ptr<const ImageView> imageView);
    };
} // namespace magma::descriptor

#include "variableCountImageDescriptors.inl"
