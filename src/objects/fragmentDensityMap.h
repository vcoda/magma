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
#include "image2D.h"

namespace magma
{
    /* An image can be bound as a fragment density map attachment
       to a render pass. This image contains normalized (x, y)
       float component fragment density values for regions of the
       framebuffer that will be used in rasterization for every
       subpass. A float component ranges from 0.0 to 1.0 where 1.0
       means full density along that axis. Implementations use
       these values as hints to optimize rendering in areas of
       low density. */

#ifdef VK_EXT_fragment_density_map
    class FragmentDensityMap : public Image2D
    {
    public:
        explicit FragmentDensityMap(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t arrayLayers,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
        explicit FragmentDensityMap(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t arrayLayers,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            const CopyLayout& bufferLayout = {0, 0, 0},
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };
#endif // VK_EXT_fragment_density_map
} // namespace magma
