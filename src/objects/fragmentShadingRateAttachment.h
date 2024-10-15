/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
    /* Adds the ability to change the rate at which fragments
       are shaded. Rather than the usual single fragment invocation
       for each pixel covered by a primitive, multiple pixels
       can be shaded by a single fragment shader invocation.
       Each pixel in the framebuffer is assigned fragment shading
       rate by the corresponding texel in the fragment shading
       rate attachment. */

#ifdef VK_KHR_fragment_shading_rate
    class FragmentShadingRateAttachment : public Image2D
    {
    public:
        explicit FragmentShadingRateAttachment(const std::unique_ptr<CommandBuffer>& cmdBuffer,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t arrayLayers,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit FragmentShadingRateAttachment(const std::unique_ptr<CommandBuffer>& cmdBuffer,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t arrayLayers,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            const CopyLayout& bufferLayout = {0, 0, 0},
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };
#endif // VK_KHR_fragment_shading_rate
} // namespace magma
