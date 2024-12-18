/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "descriptorSet.h"

namespace magma
{
    /* Allows to create large descriptor sets containing substantially
       all of application resources, and selecting amongst those
       resources with dynamic (non-uniform) indexes in the shader.

        * Descriptors can be updated after they are bound to a command
          buffer, such that the execution of the command buffer reflects
          the most recent update to the descriptors.
        * Descriptors that are not used by any pending command buffers
          can be updated, which enables writing new descriptors for
          frame N+1 while frame N is executing.
        * Relax the requirement that all descriptors in a binding that
          is "statically used" must be valid, such that descriptors that
          are not accessed by a submission need not be valid and can be
          updated while that submission is executing.
        * The final binding in a descriptor set layout can have a
          variable size. */

#ifdef VK_EXT_descriptor_indexing
    class VariableCountDescriptorSet : public DescriptorSet
    {
    public:
        explicit VariableCountDescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool,
            DescriptorSetTable& setTable,
            VkShaderStageFlags stageFlags,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            const std::unique_ptr<IShaderReflectionFactory>& shaderReflectionFactory = nullptr,
            const std::string& shaderFileName = std::string(),
            uint32_t setIndex = 0,
            const StructureChain& extendedInfo = StructureChain());
    };
#endif // VK_EXT_descriptor_indexing
}
