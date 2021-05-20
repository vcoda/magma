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
    class IAllocator;

    namespace reflection
    {
        class DescriptorSetLayoutBinding;

        class DescriptorSetLayout
        {
        public:
            template<class... DescriptorSetLayoutBinding>
            DescriptorSetLayout(DescriptorSetLayoutBinding&&... args)
            {
                std::initializer_list<int>{
                    (bindings.push_back(std::forward<DescriptorSetLayoutBinding>(args)), void(), 0)...
                };
            }
            void constructSetLayout(std::shared_ptr<magma::Device> device,
                uint32_t stageFlags,
                std::shared_ptr<IAllocator> allocator /* nullptr */);
            const std::vector<DescriptorSetLayoutBinding *> getBindings() const noexcept { return bindings; }
            std::shared_ptr<magma::DescriptorSetLayout> getLayout() const noexcept { return layout; }

        private:
            std::vector<DescriptorSetLayoutBinding *> bindings;
            std::shared_ptr<magma::DescriptorSetLayout> layout;
        };

        class DescriptorSet : public NonDispatchable<VkDescriptorSet>
        {
        public:
            explicit DescriptorSet(std::shared_ptr<magma::DescriptorPool> descriptorPool,
                DescriptorSetLayout& setLayout,
                uint32_t stageFlags,
                std::shared_ptr<IAllocator> allocator = nullptr);
            ~DescriptorSet();
            bool dirty() const noexcept;
            void update();

        private:
            std::shared_ptr<magma::DescriptorPool> descriptorPool;
            std::vector<DescriptorSetLayoutBinding *> bindings;
        };

        MAGMA_TYPEDEF_SHARED_PTR(DescriptorSet)
    } // namespace reflection
} // namespace magma

#define MAGMA_REFLECT(Type, x, ...) Type() : magma::reflection::DescriptorSetLayout(x, __VA_ARGS__) {}
