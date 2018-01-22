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
#include <vector>
#include "handle.h"
#include "../descriptors/bindings.h"

namespace magma
{
    class Device;
    class Buffer;
    class Sampler;
    class ImageView;

    class DescriptorSet : public NonDispatchable<VkDescriptorSet>
    {
        DescriptorSet(VkDescriptorSet handle,
            std::shared_ptr<const Device> device);
        friend class DescriptorPool;

    public:
        void update(const LayoutBinding& binding,
            std::shared_ptr<const Buffer> buffer) noexcept;
        void update(const LayoutBinding& binding,
            std::shared_ptr<const ImageView> imageView,
            std::shared_ptr<const Sampler> sampler) noexcept;
    };
} // namespace magma
