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
#include <initializer_list>
#include "../api/vulkan.h"
#include "../internal/nonCopyable.h"

namespace magma
{
    struct VertexInputBinding : VkVertexInputBindingDescription
    {
        constexpr VertexInputBinding(uint32_t binding, 
            uint32_t stride,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX) noexcept;
    };

    struct VertexInputAttribute : VkVertexInputAttributeDescription
    {
        constexpr VertexInputAttribute(uint32_t location, 
            uint32_t binding, 
            VkFormat format, 
            uint32_t offset) noexcept;
        constexpr VertexInputAttribute(uint32_t location,
            const VertexInputAttribute& attrib,
            uint32_t offset) noexcept;
    };

    /* Applications specify vertex input attribute and vertex input binding descriptions
       as part of graphics pipeline creation. */

    struct VertexInputState : public VkPipelineVertexInputStateCreateInfo, internal::ConstexprNonCopyable
    {
    public:
        constexpr VertexInputState() noexcept;
        constexpr VertexInputState(const VertexInputBinding& binding,
            const VertexInputAttribute& attribute);
        constexpr VertexInputState(const VertexInputBinding& binding,
            const std::initializer_list<VertexInputAttribute>& attributes);
        constexpr VertexInputState(const std::initializer_list<VertexInputBinding>& bindings,
            const std::initializer_list<VertexInputAttribute>& attributes);
        constexpr size_t hash() const noexcept;
        bool operator==(const VertexInputState&) const noexcept;
    };

    /* Managed vertex input state takes care about array of vertex attributes and 
       thereof is copyable, but not constexpr-constructible. */

    class ManagedVertexInputState final : public VertexInputState
    {
    public:
        ManagedVertexInputState(const VertexInputBinding& binding,
            const VertexInputAttribute& attribute);
        ManagedVertexInputState(const VertexInputBinding& binding,
            const std::initializer_list<VertexInputAttribute>& attributes);
        ManagedVertexInputState(
            const std::initializer_list<VertexInputBinding>& bindings,
            const std::initializer_list<VertexInputAttribute>& attributes);
        ManagedVertexInputState(const ManagedVertexInputState&);
        ManagedVertexInputState& operator=(const ManagedVertexInputState&);
        ~ManagedVertexInputState();
        size_t hash() const noexcept;
        bool operator==(const ManagedVertexInputState&) const noexcept;
    };
} // namespace magma

#include "vertexInputState.inl"

namespace magma
{
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

        constexpr VertexInputAttribute xFloat64(0, 0, VK_FORMAT_R64_SFLOAT, 0);
        constexpr VertexInputAttribute xyFloat64(0, 0, VK_FORMAT_R64G64_SFLOAT, 0);
        constexpr VertexInputAttribute xyzFloat64(0, 0, VK_FORMAT_R64G64B64_SFLOAT, 0);
        constexpr VertexInputAttribute xyzwFloat64(0, 0, VK_FORMAT_R64G64B64A64_SFLOAT, 0);

        constexpr VertexInputAttribute rgbaUNorm8(0, 0, VK_FORMAT_R8G8B8A8_UNORM, 0);
        constexpr VertexInputAttribute bgraUNorm8(0, 0, VK_FORMAT_B8G8R8A8_UNORM, 0);
    };

    namespace renderstates
    {
        constexpr VertexInputState nullVertexInput;

        constexpr VertexInputState pos1Half(VertexInputBinding(0, sizeof(short)),     attributes::xFloat16);
        constexpr VertexInputState pos2Half(VertexInputBinding(0, sizeof(short) * 2), attributes::xyFloat16);
        constexpr VertexInputState pos3Half(VertexInputBinding(0, sizeof(short) * 3), attributes::xyzFloat16);
        constexpr VertexInputState pos4Half(VertexInputBinding(0, sizeof(short) * 4), attributes::xyzwFloat16);

        constexpr VertexInputState pos1Float(VertexInputBinding(0, sizeof(float)),     attributes::xFloat32);
        constexpr VertexInputState pos2Float(VertexInputBinding(0, sizeof(float) * 2), attributes::xyFloat32);
        constexpr VertexInputState pos3Float(VertexInputBinding(0, sizeof(float) * 3), attributes::xyzFloat32);
        constexpr VertexInputState pos4Float(VertexInputBinding(0, sizeof(float) * 4), attributes::xyzwFloat32);

        constexpr VertexInputState pos1Double(VertexInputBinding(0, sizeof(double)),     attributes::xFloat64);
        constexpr VertexInputState pos2Double(VertexInputBinding(0, sizeof(double) * 2), attributes::xyFloat64);
        constexpr VertexInputState pos3Double(VertexInputBinding(0, sizeof(double) * 3), attributes::xyzFloat64);
        constexpr VertexInputState pos4Double(VertexInputBinding(0, sizeof(double) * 4), attributes::xyzwFloat64);

        constexpr VertexInputState pos2FloatTex2Float(VertexInputBinding(0, sizeof(float) * 4), {
            VertexInputAttribute(0, attributes::xyFloat32, 0),
            VertexInputAttribute(1, attributes::xyFloat32, 8)
        });
        constexpr VertexInputState pos2FloatColor3Float(VertexInputBinding(0, sizeof(float) * 5), {
            VertexInputAttribute(0, attributes::xyFloat32, 0),
            VertexInputAttribute(1, attributes::xyzFloat32, 8)
        });
        constexpr VertexInputState pos2FloatColor4Float(VertexInputBinding(0, sizeof(float) * 6), {
            VertexInputAttribute(0, attributes::xyFloat32, 0),
            VertexInputAttribute(1, attributes::xyzwFloat32, 8)
        });
        constexpr VertexInputState pos2FloatColor4UNorm(VertexInputBinding(0, sizeof(float) * 2 + sizeof(char) * 4), {
            VertexInputAttribute(0, attributes::xyFloat32, 0),
            VertexInputAttribute(1, attributes::rgbaUNorm8, 8)
        });

        constexpr VertexInputState pos3FloatTex2Float(VertexInputBinding(0, sizeof(float) * 5), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::xyFloat32, 12)
        });
        constexpr VertexInputState pos3FloatColor3Float(VertexInputBinding(0, sizeof(float) * 6), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::xyzFloat32, 12)
        });
        constexpr VertexInputState pos3FloatColor4Float(VertexInputBinding(0, sizeof(float) * 7), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::xyzwFloat32, 12)
        });
        constexpr VertexInputState pos3FloatColor4UNorm(VertexInputBinding(0, sizeof(float) * 3 + sizeof(char) * 4), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::rgbaUNorm8, 12)
        });

        constexpr VertexInputState pos3FloatNormal3Float(VertexInputBinding(0, sizeof(float) * 6), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::xyzFloat32, 12)
        });
        constexpr VertexInputState pos3FloatNormal3FloatTex2Float(VertexInputBinding(0, sizeof(float) * 8), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::xyzFloat32, 12),
            VertexInputAttribute(2, attributes::xyFloat32, 24)
        });
        constexpr VertexInputState pos3FloatNormal3FloatColor3Float(VertexInputBinding(0, sizeof(float) * 9), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::xyzFloat32, 12),
            VertexInputAttribute(2, attributes::xyzFloat32, 24)
        });
        constexpr VertexInputState pos3FloatNormal3FloatColor4Float(VertexInputBinding(0, sizeof(float) * 10), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::xyzFloat32, 12),
            VertexInputAttribute(2, attributes::xyzwFloat32, 24)
        });
        constexpr VertexInputState pos3FloatNormal3FloatColor4UNorm(VertexInputBinding(0, sizeof(float) * 6 + sizeof(char) * 4), {
            VertexInputAttribute(0, attributes::xyzFloat32, 0),
            VertexInputAttribute(1, attributes::xyzFloat32, 12),
            VertexInputAttribute(2, attributes::rgbaUNorm8, 24)
        });
    } // namespace renderstates
} // namespace magma
