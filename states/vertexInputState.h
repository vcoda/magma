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
#include "../api/vulkan.h"

namespace magma
{
    struct VertexInputBinding : VkVertexInputBindingDescription
    {
        constexpr VertexInputBinding(uint32_t binding, 
            uint32_t stride,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX) noexcept
        {
            this->binding = binding;
            this->stride = stride;
            this->inputRate = inputRate;
        }
    };

    struct VertexInputAttribute : VkVertexInputAttributeDescription
    {
        constexpr VertexInputAttribute(uint32_t location, 
            uint32_t binding, 
            VkFormat format, 
            uint32_t offset) noexcept
        {
            this->location = location;
            this->binding = binding;
            this->format = format;
            this->offset = offset;
        }

        constexpr VertexInputAttribute(uint32_t location,
            const VertexInputAttribute& attrib,
            uint32_t offset) noexcept
        {
            this->location = location;
            binding = attrib.binding;
            format = attrib.format;
            this->offset = offset;
        }
    };

    /* Applications specify vertex input attribute and vertex input binding descriptions
       as part of graphics pipeline creation. */

    class VertexInputState final : public VkPipelineVertexInputStateCreateInfo
    {
    public:
        VertexInputState() noexcept;
        VertexInputState(const VertexInputBinding& binding,
            const VertexInputAttribute& attribute);
        VertexInputState(const VertexInputBinding& binding,
            const std::initializer_list<VertexInputAttribute>& attributes);
        VertexInputState(
            const std::initializer_list<VertexInputBinding>& bindings,
            const std::initializer_list<VertexInputAttribute>& attributes);
        VertexInputState(const VertexInputState&);
        VertexInputState& operator=(const VertexInputState&);
        ~VertexInputState();
        size_t hash() const noexcept;
        bool operator==(const VertexInputState&) const noexcept;
    };

    namespace attributes
    {
        constexpr VertexInputAttribute xFloat16(0, 0, VK_FORMAT_R16_SFLOAT, 0);
        constexpr VertexInputAttribute xyFloat16(0, 0, VK_FORMAT_R16G16_SFLOAT, 0);
        constexpr VertexInputAttribute xyzFloat16(0, 0, VK_FORMAT_R16G16B16_SFLOAT, 0);
        constexpr VertexInputAttribute xyzwFloat16(0, 0, VK_FORMAT_R16G16B16A16_SFLOAT, 0);

        constexpr VertexInputAttribute xFloat32(0, 0, VK_FORMAT_R32_SFLOAT, 0);
        constexpr VertexInputAttribute xyFloat32(0, 0, VK_FORMAT_R32G32_SFLOAT, 0);
        constexpr VertexInputAttribute xyzFloat32(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0);
        constexpr VertexInputAttribute xyzwFloat32(0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0);

        constexpr VertexInputAttribute rgbaUNorm8(0, 0, VK_FORMAT_R8G8B8A8_UNORM, 0);
        constexpr VertexInputAttribute bgraUNorm8(0, 0, VK_FORMAT_B8G8R8A8_UNORM, 0);
    };

    namespace renderstates
    {
        extern const VertexInputState nullVertexInput;

        extern const VertexInputState pos2Float;
        extern const VertexInputState pos3Float;
        extern const VertexInputState pos4Float;

        extern const VertexInputState pos2FloatTex2Float;
        extern const VertexInputState pos2FloatColor3Float;
        extern const VertexInputState pos2FloatColor4Float;
        extern const VertexInputState pos2FloatColor4UNorm;

        extern const VertexInputState pos3FloatTex2Float;
        extern const VertexInputState pos3FloatColor3Float;
        extern const VertexInputState pos3FloatColor4Float;
        extern const VertexInputState pos3FloatColor4UNorm;

        extern const VertexInputState pos3FloatNormal3Float;
        extern const VertexInputState pos3FloatNormal3FloatTex2Float;
        extern const VertexInputState pos3FloatNormal3FloatColor3Float;
        extern const VertexInputState pos3FloatNormal3FloatColor4Float;
        extern const VertexInputState pos3FloatNormal3FloatColor4UNorm;
    } // namespace renderstates
} // namespace magma
