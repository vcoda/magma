/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
    class Sampler;
    typedef std::initializer_list<std::shared_ptr<const Sampler>> ImmutableSamplerList;

    /* A descriptor set layout object is defined by an array of zero or more descriptor bindings.
       Each individual descriptor binding is specified by a descriptor type, a count (array size)
       of the number of descriptors in the binding, a set of shader stages that can access the binding,
       and (if using immutable samplers) an array of sampler descriptors. */

    class DescriptorSetLayout : public NonDispatchable<VkDescriptorSetLayout>
    {
    public:
        struct Binding : VkDescriptorSetLayoutBinding
        {
            constexpr Binding(uint32_t binding,
                const Descriptor& descriptor,
                VkShaderStageFlags stageFlags) noexcept;
            std::size_t hash() const noexcept;
        };

        /* Can be used to initialize a set of immutable samplers.
           Immutable samplers are permanently bound into the set layout and must not be changed.
           The sampler objects must not be destroyed before the final use of the set layout
           and any descriptor pools and sets created using it. */

        class SamplerBinding final : public Binding
        {
        public:
            SamplerBinding(uint32_t binding,
                const Descriptor& descriptor,
                VkShaderStageFlags stageFlags,
                const ImmutableSamplerList& immutableSamplers) noexcept;
            SamplerBinding(const SamplerBinding&) noexcept;
            SamplerBinding& operator=(const SamplerBinding&) noexcept;
            ~SamplerBinding();
            std::size_t hash() const noexcept;

        private:
            void copyImmutableSamplers() noexcept;

            std::vector<std::shared_ptr<const Sampler>> immutableSamplers;
        };

    public:
        explicit DescriptorSetLayout(std::shared_ptr<Device> device,
            const Binding& binding,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit DescriptorSetLayout(std::shared_ptr<Device> device,
            const SamplerBinding& binding,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit DescriptorSetLayout(std::shared_ptr<Device> device,
            const std::initializer_list<Binding>& bindings,
            const std::initializer_list<SamplerBinding>& samplerBindings = {},
            VkDescriptorSetLayoutCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~DescriptorSetLayout();
        uint32_t getBindingCount() const noexcept { return static_cast<uint32_t>(bindings.size()); }
        uint32_t getSamplerBindingCount() const noexcept { return static_cast<uint32_t>(samplerBindings.size()); }
        const Binding& getBinding(uint32_t index) const noexcept { return bindings[index]; }
        const SamplerBinding& getSamplerBinding(uint32_t index) const noexcept { return samplerBindings[index]; }
        std::size_t getHash() const noexcept;

    private:
        std::vector<Binding> bindings;
        std::vector<SamplerBinding> samplerBindings;
        std::size_t hash;
    };
} // namespace magma

#include "descriptorSetLayout.inl"
