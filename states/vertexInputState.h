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
#include "../misc/format.h"

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
        constexpr VertexInputState(const VertexInputAttribute& attribute);
        template<size_t VertexAttributeDescriptionCount>
        constexpr VertexInputState(const VertexInputAttribute (&attributes)[VertexAttributeDescriptionCount]);
        constexpr VertexInputState(const VertexInputBinding& binding,
            const VertexInputAttribute& attribute);
        template<size_t VertexAttributeDescriptionCount>
        constexpr VertexInputState(const VertexInputBinding& binding,
            const VertexInputAttribute (&attributes)[VertexAttributeDescriptionCount]);
        template<size_t VertexBindingDescriptionCount, size_t VertexAttributeDescriptionCount>
        constexpr VertexInputState(const VertexInputBinding (&bindings)[VertexBindingDescriptionCount],
            const VertexInputAttribute (&attributes)[VertexAttributeDescriptionCount]);
        constexpr uint32_t stride(uint32_t binding) const noexcept;
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
        constexpr VertexInputAttribute xHalf(0, 0, VK_FORMAT_R16_SFLOAT, 0);
        constexpr VertexInputAttribute xyHalf(0, 0, VK_FORMAT_R16G16_SFLOAT, 0);
        constexpr VertexInputAttribute xyzHalf(0, 0, VK_FORMAT_R16G16B16_SFLOAT, 0);
        constexpr VertexInputAttribute xyzwHalf(0, 0, VK_FORMAT_R16G16B16A16_SFLOAT, 0);

        constexpr VertexInputAttribute xFloat(0, 0, VK_FORMAT_R32_SFLOAT, 0);
        constexpr VertexInputAttribute xyFloat(0, 0, VK_FORMAT_R32G32_SFLOAT, 0);
        constexpr VertexInputAttribute xyzFloat(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0);
        constexpr VertexInputAttribute xyzwFloat(0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0);

        constexpr VertexInputAttribute rgbaUNorm8(0, 0, VK_FORMAT_R8G8B8A8_UNORM, 0);
        constexpr VertexInputAttribute bgraUNorm8(0, 0, VK_FORMAT_B8G8R8A8_UNORM, 0);

        constexpr VertexInputAttribute pos2FTex2F[] = {
            VertexInputAttribute(0, xyFloat, 0),
            VertexInputAttribute(1, xyFloat, 8)};
        constexpr VertexInputAttribute pos2FCol3F[] = {
            VertexInputAttribute(0, xyFloat, 0),
            VertexInputAttribute(1, xyzFloat, 8)};
        constexpr VertexInputAttribute pos2FCol4F[] = { 
            VertexInputAttribute(0, xyFloat, 0),
            VertexInputAttribute(1, xyzwFloat, 8)};
        constexpr VertexInputAttribute pos2FCol4U8[2] = {
            VertexInputAttribute(0, xyFloat, 0),
            VertexInputAttribute(1, rgbaUNorm8, 8)};

        constexpr VertexInputAttribute pos3FTex2F[] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, xyFloat, 12)};
        constexpr VertexInputAttribute pos3FCol3F[] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, xyzFloat, 12)};
        constexpr VertexInputAttribute pos3FCol4F[] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, xyzwFloat, 12)};
        constexpr VertexInputAttribute pos3FCol4U8[2] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, rgbaUNorm8, 12)};

        constexpr VertexInputAttribute pos3FNrm3F[] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, xyzFloat, 12)};
        constexpr VertexInputAttribute pos3FNrm3FTex2F[] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, xyzFloat, 12),
            VertexInputAttribute(2, xyFloat, 24)};
        constexpr VertexInputAttribute pos3FNrm3FCol3F[] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, xyzFloat, 12),
            VertexInputAttribute(2, xyzFloat, 24)};
        constexpr VertexInputAttribute pos3FNrm3FCol4F[] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, xyzFloat, 12),
            VertexInputAttribute(2, xyzwFloat, 24)};
        constexpr VertexInputAttribute pos3FNrm3FCol4U8[] = {
            VertexInputAttribute(0, xyzFloat, 0),
            VertexInputAttribute(1, xyzFloat, 12),
            VertexInputAttribute(2, rgbaUNorm8, 24)};
    } // namespace attributes

    namespace renderstates
    {
        constexpr VertexInputState nullVertexInput;

        constexpr VertexInputState pos1H(attributes::xHalf);
        constexpr VertexInputState pos2H(attributes::xyHalf);
        constexpr VertexInputState pos3H(attributes::xyzHalf);
        constexpr VertexInputState pos4H(attributes::xyzwHalf);

        constexpr VertexInputState pos1F(attributes::xFloat);
        constexpr VertexInputState pos2F(attributes::xyFloat);
        constexpr VertexInputState pos3F(attributes::xyzFloat);
        constexpr VertexInputState pos4F(attributes::xyzwFloat);

        constexpr VertexInputState pos2FTex2F(attributes::pos2FTex2F);
        constexpr VertexInputState pos2FCol3F(attributes::pos2FCol3F);
        constexpr VertexInputState pos2FCol4F(attributes::pos2FCol4F);
        constexpr VertexInputState pos2FCol4U8(attributes::pos2FCol4U8);

        constexpr VertexInputState pos3FTex2F(attributes::pos3FTex2F);
        constexpr VertexInputState pos3FCol3F(attributes::pos3FCol3F);
        constexpr VertexInputState pos3FCol4F(attributes::pos3FCol4F);
        constexpr VertexInputState pos3FCol4U8(attributes::pos3FCol4U8);

        constexpr VertexInputState pos3FNrm3F(attributes::pos3FNrm3F);
        constexpr VertexInputState pos3FNrm3FTex2F(attributes::pos3FNrm3FTex2F);
        constexpr VertexInputState pos3FNrm3FCol3F(attributes::pos3FNrm3FCol3F);
        constexpr VertexInputState pos3FNrm3FCol4F(attributes::pos3FNrm3FCol4F);
        constexpr VertexInputState pos3FNrm3FCol4U8(attributes::pos3FNrm3FCol4U8);
    } // namespace renderstates
} // namespace magma
