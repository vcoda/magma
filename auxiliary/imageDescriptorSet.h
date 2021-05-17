/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../core/noncopyable.h"
#include "../core/enumClassArray.h"

namespace magma
{
    class Device;
    class DescriptorPool;
    class DescriptorSet;
    class DescriptorSetLayout;
    class ImageView;
    class Sampler;
    class IAllocator;

    namespace aux
    {
        enum class ImageType : uint8_t { Combined, Storage, Max };

        /* Manages descriptor sets for diffirent image types (combined, storage, etc).
           Image is implied to be bound to binding 0 in the fragment shader. */

        class ImageDescriptorSet : public core::NonCopyable
        {
        public:
            explicit ImageDescriptorSet(std::shared_ptr<Device> device, 
                std::shared_ptr<IAllocator> allocator = nullptr);
            ~ImageDescriptorSet();
            std::shared_ptr<DescriptorSetLayout> getLayout(ImageType imageType) const noexcept { return descriptorSetLayouts[imageType]; }
            std::shared_ptr<DescriptorSet> getSet(ImageType imageType) const noexcept { return descriptorSets[imageType]; }
            void writeDescriptor(std::shared_ptr<const ImageView> imageView,
                std::shared_ptr<Sampler> sampler);

        private:
            std::shared_ptr<DescriptorPool> descriptorPool;
            core::EnumClassArray<std::shared_ptr<DescriptorSetLayout>, ImageType> descriptorSetLayouts;
            core::EnumClassArray<std::shared_ptr<DescriptorSet>, ImageType> descriptorSets;
        };
    } // namespace aux
} // namespace magma
