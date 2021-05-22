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
#include "../objects/baseDescriptorSet.h"

namespace magma
{
    class ShaderReflection;
    class IAllocator;

    namespace reflection
    {
        class DescriptorSetLayout;
        class DescriptorSetLayoutBinding;
        class IShaderReflectionFactory;

        /*  A descriptor set object is an opaque object that contains storage for a set of descriptors,
            where the types and number of descriptors is defined by a descriptor set layout.
            The layout object may be used to define the association of each descriptor binding
            with memory or other implementation resources. The layout is used both for determining
            the resources that need to be associated with the descriptor set, and determining
            the interface between shader stages and shader resources. */

        class DescriptorSet : public BaseDescriptorSet
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
            virtual bool dirty() const noexcept override;
            virtual void update() override;

        private:
            void validateReflection(std::shared_ptr<const ShaderReflection> shaderReflection) const;

            std::vector<DescriptorSetLayoutBinding *> layoutBindings;
        };

        MAGMA_TYPEDEF_SHARED_PTR(DescriptorSet)
    } // namespace reflection
} // namespace magma
