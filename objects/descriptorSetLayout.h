/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    /* A descriptor set layout object is defined by an array of zero or more descriptor bindings.
       Each individual descriptor binding is specified by a descriptor type, a count (array size)
       of the number of descriptors in the binding, a set of shader stages that can access the binding,
       and (if using immutable samplers) an array of sampler descriptors. */

    class DescriptorSetLayout : public NonDispatchable<VkDescriptorSetLayout>
    {
    public:
        explicit DescriptorSetLayout(std::shared_ptr<Device> device,
            const std::vector<VkDescriptorSetLayoutBinding>& bindings,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());

        /* If the descriptor set layout exceeds the
           VkPhysicalDeviceMaintenance3Properties::maxPerSetDescriptors limit,
           whether the descriptor set layout is supported is implementation-dependent
           and may depend on whether the descriptor sizes and alignments
           cause the layout to exceed an internal limit. */

    #ifdef VK_KHR_maintenance3
        struct Support
        {
            bool supported = false;
            uint32_t maxVariableDescriptorCount = 0;
        };

        explicit DescriptorSetLayout(std::shared_ptr<Device> device,
            const std::vector<VkDescriptorSetLayoutBinding>& bindings,
            Support& supportResult,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
    #endif // VK_KHR_maintenance3
        ~DescriptorSetLayout();
        hash_t getHash() const noexcept { return hash; }

    private:
        hash_t hash;
    };
} // namespace magma
