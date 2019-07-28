/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../descriptors/descriptors.h"

namespace magma
{
    typedef std::initializer_list<std::shared_ptr<class Sampler>> ImmutableSamplerList;

    /* A descriptor set layout object is defined by an array of zero or more descriptor bindings.
       Each individual descriptor binding is specified by a descriptor type, a count (array size)
       of the number of descriptors in the binding, a set of shader stages that can access the binding,
       and (if using immutable samplers) an array of sampler descriptors. */

    class DescriptorSetLayout : public NonDispatchable<VkDescriptorSetLayout>
    {
    public:
        struct Binding : VkDescriptorSetLayoutBinding
        {
            explicit Binding(uint32_t binding,
                const Descriptor& descriptor,
                VkShaderStageFlags stageFlags,
                const ImmutableSamplerList& immutableSamplers = {}) noexcept;
            ~Binding();
        };

    public:
        explicit DescriptorSetLayout(std::shared_ptr<Device> device,
            const Binding& binding,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit DescriptorSetLayout(std::shared_ptr<Device> device,
            const std::initializer_list<Binding>& bindings,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~DescriptorSetLayout();
        uint32_t getBindingCount() const noexcept { return static_cast<uint32_t>(bindings.size()); }
        const Binding& getBinding(uint32_t index) const noexcept { return bindings[index]; }

    private:
        std::vector<Binding> bindings;
    };
} // namespace magma
