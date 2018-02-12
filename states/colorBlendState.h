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
#include "../vulkan.h"

namespace magma
{
    struct ColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
        ColorBlendAttachmentState(bool blendEnable, 
            VkBlendFactor srcBlendFactor,
            VkBlendFactor dstBlendFactor,
            VkBlendOp blendOp, 
            VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | 
                                                   VK_COLOR_COMPONENT_G_BIT | 
                                                   VK_COLOR_COMPONENT_B_BIT | 
                                                   VK_COLOR_COMPONENT_A_BIT);
    };

    class ColorBlendState final : public VkPipelineColorBlendStateCreateInfo
    {
    public:
        ColorBlendState(const ColorBlendAttachmentState& attachment);
        ColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments);
        ColorBlendState(const ColorBlendState&);
        ColorBlendState& operator=(const ColorBlendState&);
        ~ColorBlendState();
    };

    namespace blends
    {
        extern const ColorBlendAttachmentState passR;
        extern const ColorBlendAttachmentState passRG;
        extern const ColorBlendAttachmentState passRGB;
        extern const ColorBlendAttachmentState passRGBA;

        extern const ColorBlendAttachmentState addR;
        extern const ColorBlendAttachmentState addRG;
        extern const ColorBlendAttachmentState addRGB;
        extern const ColorBlendAttachmentState addRGBA;

        extern const ColorBlendAttachmentState normalR;
        extern const ColorBlendAttachmentState normalRG;
        extern const ColorBlendAttachmentState normalRGB;
        extern const ColorBlendAttachmentState normalRGBA;
    } // namespace blends

    namespace states
    {
        extern const ColorBlendState dontBlendWriteR;
        extern const ColorBlendState dontBlendWriteRG;
        extern const ColorBlendState dontBlendWriteRGB;
        extern const ColorBlendState dontBlendWriteRGBA;

        extern const ColorBlendState blendAddWriteR;
        extern const ColorBlendState blendAddWriteRG;
        extern const ColorBlendState blendAddWriteRGB;
        extern const ColorBlendState blendAddWriteRGBA;

        extern const ColorBlendState blendNormalWriteR;
        extern const ColorBlendState blendNormalWriteRG;
        extern const ColorBlendState blendNormalWriteRGB;
        extern const ColorBlendState blendNormalWriteRGBA;
    } // namespace states
} // namespace magma
