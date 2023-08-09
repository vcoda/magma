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
#include "buffer.h"

namespace magma
{
#ifdef VK_EXT_transform_feedback
    /* Transform feedback buffer to capture the primitives
       emitted by the graphics pipeline from SPIR-V outputs
       decorated for transform feedback. The captured data
       can be drawn again where the vertex count is derived
       from the byte counter without CPU intervention. */

    class TransformFeedbackBuffer : public Buffer
    {
    public:
        explicit TransformFeedbackBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing());
    };

    /* Counter buffer is used for storing and retrieving
       a byte counter with the following methods:
        1. CommandBuffer::beginTransformFeedback()
        2. CommandBuffer::endTransformFeedback()
        3. CommandBuffer::drawIndirectByteCount() */

    class TransformFeedbackCounterBuffer : public Buffer
    {
    public:
        explicit TransformFeedbackCounterBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing());
    };
#endif // VK_EXT_transform_feedback
} // namespace magma
