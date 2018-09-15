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
    struct VertexInputBinding : VkVertexInputBindingDescription
    {
        VertexInputBinding(uint32_t binding, uint32_t stride,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX) noexcept
        {
            this->binding = binding;
            this->stride = stride;
            this->inputRate = inputRate;
        }
    };

    struct VertexInputAttribute : VkVertexInputAttributeDescription
    {
        VertexInputAttribute(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset) noexcept
        {
            this->location = location;
            this->binding = binding;
            this->format = format;
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

    namespace states
    {
        extern const VertexInputState nullVertexInput;

        extern const VertexInputState pos2Float;
        extern const VertexInputState pos3Float;
        extern const VertexInputState pos4Float;

        extern const VertexInputState pos2Float_Tex2Float;
        extern const VertexInputState pos2Float_Col3Float;
        extern const VertexInputState pos2Float_Col4Float;
        extern const VertexInputState pos2Float_Col4UNorm;

        extern const VertexInputState pos3Float_Tex2Float;
        extern const VertexInputState pos3Float_Col3Float;
        extern const VertexInputState pos3Float_Col4Float;
        extern const VertexInputState pos3Float_Col4UNorm;

        extern const VertexInputState pos3Float_Nrm3Float;
        extern const VertexInputState pos3Float_Nrm3Float_Col3Float;
        extern const VertexInputState pos3Float_Nrm3Float_Col4Float;
        extern const VertexInputState pos3Float_Nrm3Float_Col4UNorm;
        extern const VertexInputState pos3Float_Nrm3Float_Tex2Float;
    } // namespace states
} // namespace magma
