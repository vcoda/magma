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
#include "../internal/hash.h"

namespace magma
{
constexpr AdvancedColorBlendAttachmentState::AdvancedColorBlendAttachmentState(VkBlendOp advancedBlendOp,
    VkColorComponentFlags colorWriteMask /* R, G, B, A */) noexcept
{
    blendEnable = VK_TRUE;
    srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendOp = advancedBlendOp;
    srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    alphaBlendOp = advancedBlendOp;
    this->colorWriteMask = colorWriteMask;
}

constexpr size_t AdvancedColorBlendAttachmentState::hash() const noexcept
{
    return internal::hashArgs(
        blendEnable,
        srcColorBlendFactor,
        dstColorBlendFactor,
        colorBlendOp,
        srcAlphaBlendFactor,
        dstAlphaBlendFactor,
        alphaBlendOp,
        colorWriteMask);
}

constexpr bool AdvancedColorBlendAttachmentState::operator==(const AdvancedColorBlendAttachmentState& other) const noexcept
{
    return (blendEnable == other.blendEnable) &&
        (srcColorBlendFactor == other.srcColorBlendFactor) &&
        (dstColorBlendFactor == other.dstColorBlendFactor) &&
        (colorBlendOp == other.colorBlendOp) &&
        (srcAlphaBlendFactor == other.srcAlphaBlendFactor) &&
        (dstAlphaBlendFactor == other.dstAlphaBlendFactor) &&
        (alphaBlendOp == other.alphaBlendOp) &&
        (colorWriteMask == other.colorWriteMask);
}
} // namespace magma
