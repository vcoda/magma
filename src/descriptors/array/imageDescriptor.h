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
#include "descriptor.h"
#include "../../objects/sampler.h"
#include "../../objects/image.h"
#include "../../objects/imageView.h"

namespace magma
{
    namespace descriptor
    {
        typedef std::pair<std::shared_ptr<const ImageView>,
            std::shared_ptr<const magma::Sampler>> ImageSampler;

        namespace array
        {
            /* Descriptor element of sampler array. */

            class SamplerDescriptor : Descriptor<VkDescriptorImageInfo>
            {
            public:
                explicit SamplerDescriptor(VkDescriptorImageInfo& descriptor, bool& updated) noexcept;
                void operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept;
            };

            /* Descriptor element of image array. */

            class ImageDescriptor : BaseImageDescriptor
            {
            public:
                explicit ImageDescriptor(VkDescriptorImageInfo& descriptor,
                     VkImageType& imageType, VkImageUsageFlags requiredUsage, bool& updated) noexcept;
                void operator=(std::shared_ptr<const ImageView> imageView) noexcept;
            };

            /* Descriptor element of combined image/sampler array. */

            class ImageSamplerDescriptor : BaseImageDescriptor
            {
            public:
                explicit ImageSamplerDescriptor(VkDescriptorImageInfo& descriptor,
                    VkImageType& imageType, VkImageUsageFlags requiredUsage, bool& updated) noexcept;
                void operator=(const ImageSampler&) noexcept;
            };

            /* Descriptor element of combined image/immutable sampler array. */

            class ImageImmutableSamplerDescriptor : BaseImageDescriptor
            {
            public:
                explicit ImageImmutableSamplerDescriptor(VkDescriptorImageInfo& descriptor, VkSampler& immutableSampler,
                    VkImageType& imageType, VkImageUsageFlags requiredUsage, bool& updated) noexcept;
                void operator=(const ImageSampler&) noexcept;
                void operator=(std::shared_ptr<const ImageView>) noexcept;

            private:
                VkSampler& immutableSampler;
            };
        } // namespace array
    } // namespace descriptor
} // namespace magma

#include "imageDescriptor.inl"
