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
#include "colorMask.h"

namespace magma
{
    /* Per-target blending state for each individual color attachment. */

    struct ColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
        constexpr ColorBlendAttachmentState(VkColorComponentFlags colorWriteMask = colormask::none) noexcept;
        constexpr ColorBlendAttachmentState(VkBlendFactor srcBlendFactor,
            VkBlendFactor dstBlendFactor,
            VkBlendOp blendOp,
            VkColorComponentFlags colorWriteMask = colormask::rgba) noexcept;
        constexpr ColorBlendAttachmentState(VkBlendFactor srcColorBlendFactor,
            VkBlendFactor dstColorBlendFactor,
            VkBlendOp colorBlendOp,
            VkBlendFactor srcAlphaBlendFactor,
            VkBlendFactor dstAlphaBlendFactor,
            VkBlendOp alphaBlendOp,
            VkColorComponentFlags colorWriteMask = colormask::rgba) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const ColorBlendAttachmentState&) const noexcept;
    };
} // namespace magma

#include "colorBlendAttachmentState.inl"

namespace magma
{
#define MAGMA_DEFINE_COLOR_WRITE_ATTACHMENT_STATE_PERMUTATIONS(name)\
    constexpr ColorBlendAttachmentState name##R(colormask::r);\
    constexpr ColorBlendAttachmentState name##G(colormask::g);\
    constexpr ColorBlendAttachmentState name##B(colormask::b);\
    constexpr ColorBlendAttachmentState name##A(colormask::a);\
    constexpr ColorBlendAttachmentState name##Rg(colormask::rg);\
    constexpr ColorBlendAttachmentState name##Rb(colormask::rb);\
    constexpr ColorBlendAttachmentState name##Ra(colormask::ra);\
    constexpr ColorBlendAttachmentState name##Gb(colormask::gb);\
    constexpr ColorBlendAttachmentState name##Ga(colormask::ga);\
    constexpr ColorBlendAttachmentState name##Ba(colormask::ba);\
    constexpr ColorBlendAttachmentState name##Rgb(colormask::rgb);\
    constexpr ColorBlendAttachmentState name##Rga(colormask::rga);\
    constexpr ColorBlendAttachmentState name##Rba(colormask::rba);\
    constexpr ColorBlendAttachmentState name##Gba(colormask::gba);\
    constexpr ColorBlendAttachmentState name##Rgba(colormask::rgba);

#define MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(name, srcBlend, dstBlend, blendOp)\
    constexpr ColorBlendAttachmentState name##R(srcBlend, dstBlend, blendOp, colormask::r);\
    constexpr ColorBlendAttachmentState name##G(srcBlend, dstBlend, blendOp, colormask::g);\
    constexpr ColorBlendAttachmentState name##B(srcBlend, dstBlend, blendOp, colormask::b);\
    constexpr ColorBlendAttachmentState name##A(srcBlend, dstBlend, blendOp, colormask::a);\
    constexpr ColorBlendAttachmentState name##Rg(srcBlend, dstBlend, blendOp, colormask::rg);\
    constexpr ColorBlendAttachmentState name##Rb(srcBlend, dstBlend, blendOp, colormask::rb);\
    constexpr ColorBlendAttachmentState name##Ra(srcBlend, dstBlend, blendOp, colormask::ra);\
    constexpr ColorBlendAttachmentState name##Gb(srcBlend, dstBlend, blendOp, colormask::gb);\
    constexpr ColorBlendAttachmentState name##Ga(srcBlend, dstBlend, blendOp, colormask::ga);\
    constexpr ColorBlendAttachmentState name##Ba(srcBlend, dstBlend, blendOp, colormask::ba);\
    constexpr ColorBlendAttachmentState name##Rgb(srcBlend, dstBlend, blendOp, colormask::rgb);\
    constexpr ColorBlendAttachmentState name##Rga(srcBlend, dstBlend, blendOp, colormask::rga);\
    constexpr ColorBlendAttachmentState name##Rba(srcBlend, dstBlend, blendOp, colormask::rba);\
    constexpr ColorBlendAttachmentState name##Gba(srcBlend, dstBlend, blendOp, colormask::gba);\
    constexpr ColorBlendAttachmentState name##Rgba(srcBlend, dstBlend, blendOp, colormask::rgba);

    namespace blendstate
    {
        constexpr ColorBlendAttachmentState writeNone(colormask::none);

        MAGMA_DEFINE_COLOR_WRITE_ATTACHMENT_STATE_PERMUTATIONS(write)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(add, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(subtract, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(reverseSubtract, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(min, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(max, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(multiply, VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(normal, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(normalPremultiplied, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD)
        MAGMA_DEFINE_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(screen, VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD) // 1 - (1 - a) (1 - b) = a * (1 - b) + b = a + (1 - a) * b
    } // namespace blendstate
} // namespace magma
