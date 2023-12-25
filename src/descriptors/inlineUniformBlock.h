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
#ifdef VK_EXT_inline_uniform_block
#include "descriptor.h"

namespace magma
{
    namespace descriptor
    {
        /* An inline uniform block is almost identical to a uniform buffer,
           and differs only in taking its storage directly from the
           encompassing descriptor set instead of being backed by buffer
           memory. It is typically used to access a small set of constant
           data that does not require the additional flexibility provided
           by the indirection enabled when using a uniform buffer where
           the descriptor and the referenced buffer memory are decoupled.
           Compared to push constants, they allow reusing the same set of
           constant data across multiple disjoint sets of drawing and
           dispatching commands. */

        template<class UniformBlockType>
        class InlineUniformBlock : public Descriptor<VkWriteDescriptorSetInlineUniformBlockEXT>
        {
        public:
            InlineUniformBlock(uint32_t binding) noexcept;
            bool associatedWithResource() const noexcept override;
            void write(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;
            InlineUniformBlock<UniformBlockType>& operator=(const UniformBlockType&) noexcept;
        };
    } // namespace descriptor
} // namespace magma

#include "inlineUniformBlock.inl"
#endif // VK_EXT_inline_uniform_block
