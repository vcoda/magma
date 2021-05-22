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
#include "../objects/nondispatchable.h"

namespace magma
{
    class Device;
    class DescriptorPool;
    class DescriptorSetLayout;
    class ShaderReflection;
    class IAllocator;

    namespace reflection
    {
        class DescriptorSetLayout;
        class DescriptorSetLayoutBinding;
        class IShaderReflectionFactory;

        /* Descriptor set. */

        class DescriptorSet : public NonDispatchable<VkDescriptorSet>
        {
        public:
            explicit DescriptorSet(std::shared_ptr<magma::DescriptorPool> descriptorPool,
                const DescriptorSetLayout& setLayout,
                uint32_t setIndex,
                uint32_t stageFlags,
                std::shared_ptr<IAllocator> allocator = nullptr,
                std::shared_ptr<IShaderReflectionFactory> shaderReflectionFactory = nullptr,
                const std::string& shaderFileName = std::string());
            ~DescriptorSet();
            std::shared_ptr<magma::DescriptorSetLayout> getLayout() const noexcept { return setLayout; }
            bool dirty() const noexcept;
            void update();

        private:
            void validateReflection(std::shared_ptr<const ShaderReflection> shaderReflection) const;

            std::shared_ptr<DescriptorPool> descriptorPool;
            std::shared_ptr<magma::DescriptorSetLayout> setLayout;
            std::vector<DescriptorSetLayoutBinding *> layoutBindings;
            uint32_t setIndex;
        };

        MAGMA_TYPEDEF_SHARED_PTR(DescriptorSet)
    } // namespace reflection
} // namespace magma
