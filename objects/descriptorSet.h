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
#include "nondispatchable.h"

namespace magma
{
    class DescriptorPool;
    class DescriptorSetLayout;
    class DescriptorSetDeclaration;
    class ShaderReflection;
    class IShaderReflectionFactory;
    class IAllocator;

    /*  A descriptor set object is an opaque object that contains storage for a set of descriptors,
        where the types and number of descriptors is defined by a descriptor set layout.
        The layout object may be used to define the association of each descriptor binding
        with memory or other implementation resources. The layout is used both for determining
        the resources that need to be associated with the descriptor set, and determining
        the interface between shader stages and shader resources. */

    class DescriptorSet : public NonDispatchable<VkDescriptorSet>
    {
    public:
        explicit DescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool,
            uint32_t setIndex,
            DescriptorSetDeclaration& setLayoutDecl,
            uint32_t stageFlags,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<IShaderReflectionFactory> shaderReflectionFactory = nullptr,
            const std::string& shaderFileName = std::string());
        ~DescriptorSet();
        uint32_t getIndex() const noexcept { return setIndex; }
        std::shared_ptr<DescriptorPool> getPool() noexcept { return descriptorPool; }
        std::shared_ptr<const DescriptorPool> getPool() const noexcept { return descriptorPool; }
        std::shared_ptr<DescriptorSetLayout> getLayout() noexcept { return setLayout; }
        std::shared_ptr<const DescriptorSetLayout> getLayout() const noexcept { return setLayout; }
        void populateWriteDescriptors(std::vector<VkWriteDescriptorSet>& writeDescriptors) const;
        bool dirty() const;

    private:
        void validateReflection(std::shared_ptr<const ShaderReflection> shaderReflection) const;

        const uint32_t setIndex;
        DescriptorSetDeclaration& setLayoutDecl;
        std::shared_ptr<DescriptorSetLayout> setLayout;
        std::shared_ptr<DescriptorPool> descriptorPool;
    };
}
