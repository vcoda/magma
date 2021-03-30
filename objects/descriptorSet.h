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
#include "../descriptors/bindings.h"

namespace magma
{
    class Device;
    class DescriptorPool;
    class DescriptorSetLayout;
    class Buffer;
    class BufferView;
    class Sampler;
    class ImageView;
#ifdef VK_NV_ray_tracing
    class AccelerationStructure;
#endif

    /*  A descriptor set object is an opaque object that contains storage for a set of descriptors,
        where the types and number of descriptors is defined by a descriptor set layout.
        The layout object may be used to define the association of each descriptor binding
        with memory or other implementation resources. The layout is used both for determining
        the resources that need to be associated with the descriptor set, and determining
        the interface between shader stages and shader resources. */

    class DescriptorSet : public NonDispatchable<VkDescriptorSet>
    {
        explicit DescriptorSet(VkDescriptorSet handle,
            std::shared_ptr<Device> device,
            std::shared_ptr<DescriptorPool> pool,
            std::shared_ptr<DescriptorSetLayout> layout) noexcept;
        friend class DescriptorPool;

    public:
        void update(uint32_t index,
            std::shared_ptr<const Buffer> buffer) noexcept;
        void update(uint32_t index,
            std::shared_ptr<const ImageView> imageView,
            std::shared_ptr<const Sampler> sampler) noexcept;
        void update(uint32_t index,
            std::shared_ptr<const BufferView> texelBufferView) noexcept;
#ifdef VK_NV_ray_tracing
        void update(uint32_t index, 
            std::shared_ptr<const AccelerationStructure> accelerationStructure) noexcept;
#endif
        std::shared_ptr<DescriptorPool> getPool() noexcept { return pool; }
        std::shared_ptr<const DescriptorPool> getPool() const noexcept { return pool; }
        std::shared_ptr<DescriptorSetLayout> getLayout() noexcept { return layout; }
        std::shared_ptr<const DescriptorSetLayout> getLayout() const noexcept { return layout; }

    private:
        std::shared_ptr<DescriptorPool> pool;
        std::shared_ptr<DescriptorSetLayout> layout;
    };
} // namespace magma
