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
#include "../objects/sampler.h"
#include "../objects/imageView.h"

namespace magma
{
    namespace descriptor
    {
        /* A sampler descriptor is a descriptor type associated with a sampler object,
           used to control the behavior of sampling operations performed on a sampled image. */

        class Sampler : public Descriptor
        {
        public:
            Sampler(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_SAMPLER, 1, binding) {}
            Sampler& operator=(std::shared_ptr<const magma::Sampler>) noexcept;
        };

        /* Immutable samplers are permanently bound into the set layout;
           later binding a sampler into an immutable sampler slot
           in a descriptor set is not allowed. */

        class ImmutableSampler : public Descriptor
        {
        public:
            ImmutableSampler(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_SAMPLER, 1, binding) {}
            ImmutableSampler& operator=(std::shared_ptr<const magma::Sampler>) noexcept;
        };

        /* A combined image sampler is a single descriptor type associated with both a sampler and an image resource,
           combining both a sampler and sampled image descriptor into a single descriptor. */

        class CombinedImageSampler : public Descriptor
        {
        public:
            CombinedImageSampler(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, binding) {}
            CombinedImageSampler& operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>&) noexcept;
        };

        /* Updates to a VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER descriptor with immutable samplers
           does not modify the samplers (the image views are updated, but the sampler updates are ignored). */

        class CombinedImageImmutableSampler : public Descriptor
        {
        public:
            CombinedImageImmutableSampler(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, binding) {}
            CombinedImageImmutableSampler& operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>&) noexcept;
            CombinedImageImmutableSampler& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* A sampled image is a descriptor type associated with an image resource
           via an image view that sampling operations can be performed on. */

        class SampledImage : public Descriptor
        {
        public:
            SampledImage(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, binding) {}
            SampledImage& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* A storage image is a descriptor type associated with an image resource
           via an image view that load, store, and atomic operations can be performed on. */

        class StorageImage : public Descriptor
        {
        public:
            StorageImage(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, binding) {}
            StorageImage& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* An input attachment is a descriptor type associated with an image resource
           via an image view that can be used for framebuffer local load operations in fragment shaders. */

        class InputAttachment : public Descriptor
        {
        public:
            InputAttachment(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, binding) {}
            InputAttachment& operator=(std::shared_ptr<const ImageView>) noexcept;
        };
    } // namespace descriptor
} // namespace magma

#include "imageDescriptor.inl"
