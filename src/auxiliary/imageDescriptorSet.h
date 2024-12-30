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
#include "../objects/descriptorSetLayout.h"

namespace magma
{
    class Device;
    class DescriptorPool;
    class DescriptorSet;
    class DescriptorSetLayout;
    class ShaderReflection;
    class ImageView;
    class Sampler;
    class IAllocator;

    namespace aux
    {
        /* Allocates descriptor set for the first image binding
           found in the shader reflection. */

        class ImageDescriptorSet final
        {
        public:
            explicit ImageDescriptorSet(std::shared_ptr<Device> device,
                const std::unique_ptr<const ShaderReflection>& reflection,
                std::shared_ptr<IAllocator> allocator = nullptr);
            ~ImageDescriptorSet();
            const std::unique_ptr<DescriptorSet>& getDescriptorSet() const noexcept { return descriptorSet; }
            void writeDescriptor(lent_ptr<const ImageView> imageView,
                lent_ptr<const Sampler> sampler);

        private:
            struct ImageTable;
            struct StorageImageTable;
            std::shared_ptr<DescriptorPool> descriptorPool;
            std::unique_ptr<DescriptorSet> descriptorSet;
            std::unique_ptr<ImageTable> imageTable;
            std::unique_ptr<StorageImageTable> storageImageTable;
            uint32_t binding;
        };
    } // namespace aux
} // namespace magma
