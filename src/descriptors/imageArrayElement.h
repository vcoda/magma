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
#include "arrayElement.h"
#include "../objects/sampler.h"
#include "../objects/image.h"
#include "../objects/imageView.h"

namespace magma
{
    namespace descriptor
    {
        typedef std::pair<std::shared_ptr<const ImageView>,
            std::shared_ptr<const magma::Sampler>> ImageSampler;
    
        /* Element of array of sampler descriptors. */

        class SamplerArrayElement final : ArrayElement
        {
        public:
            explicit SamplerArrayElement(DescriptorSetLayoutBinding *array,
                VkDescriptorImageInfo& element) noexcept;
            void operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept;

        private:
            VkDescriptorImageInfo& element;
        };

        /* Element of array of image descriptors. */

        class ImageArrayElement final : ArrayElement
        {
        public:
            explicit ImageArrayElement(DescriptorSetLayoutBinding *array,
                VkDescriptorImageInfo& element,
                VkImageUsageFlags usage) noexcept;
            void operator=(std::shared_ptr<const ImageView> imageView) noexcept;

        private:
            VkDescriptorImageInfo& element;
            const VkImageUsageFlags usage;
        };

        /* Element of array of combined image/sampler descriptors. */

        class ImageSamplerArrayElement final : ArrayElement
        {
        public:
            explicit ImageSamplerArrayElement(DescriptorSetLayoutBinding *array,
                VkDescriptorImageInfo& element,
                VkImageUsageFlags usage) noexcept;
            void operator=(const ImageSampler& imageSampler) noexcept;

        private:
            VkDescriptorImageInfo& element;
            const VkImageUsageFlags usage;
        };

        /* Element of array of combined image and (immutable) sampler descriptors. */

        class ImageImmutableSamplerArrayElement final : ArrayElement
        {
        public:
            explicit ImageImmutableSamplerArrayElement(DescriptorSetLayoutBinding *array,
                VkDescriptorImageInfo& element,
                VkSampler& immutableSampler,
                VkImageUsageFlags usage) noexcept;
            void operator=(const ImageSampler& imageSampler) noexcept;
            void operator=(std::shared_ptr<const ImageView> imageView) noexcept;

        private:
            VkDescriptorImageInfo& element;
            VkSampler& immutableSampler;
            const VkImageUsageFlags usage;
        };
    } // namespace descriptor
} // namespace magma

#include "imageArrayElement.inl"
