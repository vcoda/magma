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
#include "../descriptors/binding.h"

namespace magma
{
    class Sampler;
    typedef std::initializer_list<std::shared_ptr<const Sampler>> ImmutableSamplerList;

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
            VkDescriptorSetLayoutCreateFlags flags = 0);
        ~DescriptorSetLayout();
        std::size_t getHash() const noexcept { return hash; }

    private:
        std::size_t hash;
    };

    /* A reflection is a mechanism making it possible to investigate yourself.
       This object contains list of descriptor bindings that is used to investigate
       their formats and properties at runtime. */

    class DescriptorSetReflection
    {
    public:
        template<class... DescriptorSetLayoutBinding>
        DescriptorSetReflection(DescriptorSetLayoutBinding&&... args)
        {
            // Use "temporary array" idiom
            // https://stackoverflow.com/questions/28866559/writing-variadic-template-constructor
            std::initializer_list<int>{
                (bindings.push_back(std::forward<DescriptorSetLayoutBinding>(args)), void(), 0)...
            };
        }

        bool dirty() const noexcept
        {
            for (auto binding : bindings)
                if (binding->dirty())
                    return true;
            return false;
        }

        const std::vector<binding::DescriptorSetLayoutBinding *> getBindings() const noexcept { return bindings; }

    private:
        std::vector<binding::DescriptorSetLayoutBinding *> bindings;
    };
} // namespace magma

#define MAGMA_REFLECT(Type, x, ...) Type() : magma::DescriptorSetReflection(x, __VA_ARGS__) {}
