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

namespace magma
{
    namespace specializations
    {
        template<VkFormat format = VK_FORMAT_UNDEFINED>
        struct AttributeType
        {
            constexpr operator VkFormat() const { return format; }
        };

        /* User have to specialize this template for concrete type */

        template<typename Type, bool normalized = false>
        struct VertexAttribute : AttributeType<VK_FORMAT_UNDEFINED>
        {
            VertexAttribute() noexcept
            {
                static_assert(false, "vertex attribute type not specialized");
            }

            constexpr std::size_t size() const { return sizeof(Type); }
        };

        /* Specialization for basic machine types. */

        template<> struct VertexAttribute<uint8_t, true> : AttributeType<VK_FORMAT_R8_UNORM> {};
        template<> struct VertexAttribute<int8_t, true> : AttributeType<VK_FORMAT_R8_SNORM> {};
        template<> struct VertexAttribute<uint8_t, false> : AttributeType<VK_FORMAT_R8_UINT> {};
        template<> struct VertexAttribute<int8_t, false> : AttributeType<VK_FORMAT_R8_SINT> {};

        template<> struct VertexAttribute<uint16_t, true> : AttributeType<VK_FORMAT_R16_UNORM> {};
        template<> struct VertexAttribute<int16_t, true> : AttributeType<VK_FORMAT_R16_SNORM> {};
        template<> struct VertexAttribute<uint16_t, false> : AttributeType<VK_FORMAT_R16_UINT> {};
        template<> struct VertexAttribute<int16_t, false> : AttributeType<VK_FORMAT_R16_SINT> {};

        template<> struct VertexAttribute<uint32_t> : AttributeType<VK_FORMAT_R32_UINT> {};
        template<> struct VertexAttribute<int32_t> : AttributeType<VK_FORMAT_R32_SINT> {};
        template<> struct VertexAttribute<float> : AttributeType<VK_FORMAT_R32_SFLOAT> {};

        template<> struct VertexAttribute<uint64_t> : AttributeType<VK_FORMAT_R64_UINT> {};
        template<> struct VertexAttribute<int64_t> : AttributeType<VK_FORMAT_R64_SINT> {};
        template<> struct VertexAttribute<double> : AttributeType<VK_FORMAT_R64_SFLOAT> {};
    } // namespace specializations
} // namespace magma
