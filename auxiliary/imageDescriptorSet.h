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
#include "../descriptors/binding.h"
#include "../objects/descriptorSetLayout.h"

namespace magma
{
    class Device;
    class DescriptorPool;
    class DescriptorSetLayout;
    class ShaderReflection;
    class ImageView;
    class Sampler;
    class IAllocator;

    namespace aux
    {
        /* Allocates descriptor set for the first image binding found in the shader reflection. */

        class ImageDescriptorSet : public core::NonCopyable
        {
        public:
            explicit ImageDescriptorSet(std::shared_ptr<Device> device,
                std::shared_ptr<const ShaderReflection> reflection,
                std::shared_ptr<IAllocator> allocator = nullptr);
            ~ImageDescriptorSet();
            std::shared_ptr<DescriptorSetLayout> getLayout() const noexcept { return descriptorSetLayout; }
            std::shared_ptr<DescriptorSet> getSet() const noexcept { return descriptorSet; }
            void writeDescriptor(std::shared_ptr<const ImageView> imageView,
                std::shared_ptr<Sampler> sampler);

        private:
            struct ImageSetLayout;
            struct StorageImageSetLayout;
            std::shared_ptr<DescriptorPool> descriptorPool;
            std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
            std::shared_ptr<DescriptorSet> descriptorSet;
            std::unique_ptr<ImageSetLayout> imageSetLayout;
            std::unique_ptr<StorageImageSetLayout> storageImageSetLayout;
            uint32_t binding;
        };
    } // namespace aux
} // namespace magma
