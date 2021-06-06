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

    /* A descriptor set declaration provides a reflection mechanism making it possible to investigate yourself.
       It contains list of descriptor bindings to investigate their properties at runtime. */

    class DescriptorSetDeclaration
    {
    public:
        virtual const std::vector<binding::DescriptorSetLayoutBinding *>& getBindings() = 0;

        bool dirty()
        {
            for (auto binding : getBindings())
                if (binding->dirty())
                    return true;
            return false;
        }

    protected:
        template<class... DescriptorSetLayoutBinding>
        static std::vector<binding::DescriptorSetLayoutBinding *> buildArgsList(DescriptorSetLayoutBinding&&... args)
        {
            std::vector<binding::DescriptorSetLayoutBinding *> bindings;
            // Use "temporary array" idiom
            // https://stackoverflow.com/questions/28866559/writing-variadic-template-constructor
            std::initializer_list<int>{
                (bindings.push_back(std::forward<DescriptorSetLayoutBinding>(args)), void(), 0)...
            };
            return bindings;
        }
    };
} // namespace magma

#define MAGMA_REFLECT(...)\
virtual const std::vector<magma::binding::DescriptorSetLayoutBinding *>& getBindings() override\
{\
    static std::vector<magma::binding::DescriptorSetLayoutBinding *> bindings =\
        buildArgsList(__VA_ARGS__);\
    return bindings;\
}
