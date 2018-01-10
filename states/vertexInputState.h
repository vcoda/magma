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
#include "../shared.h"

namespace magma
{
    class VertexInputBinding
    {
    public:
        VertexInputBinding(uint32_t binding, uint32_t stride,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX)
        {
            desc.binding = binding;
            desc.stride = stride;
            desc.inputRate = inputRate;
        }

    private:
        VkVertexInputBindingDescription desc;
        friend class VertexInputState;
    };

    class VertexInputAttribute
    {
    public:
        VertexInputAttribute(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset)
        {
            desc.location = location;
            desc.binding = binding;
            desc.format = format;
            desc.offset = offset;
        }

    private:
        VkVertexInputAttributeDescription desc;
        friend class VertexInputState;
    };

    class VertexInputState final
    {
    public:
        VertexInputState(const std::vector<VertexInputBinding>& bindings,
            const std::vector<VertexInputAttribute>& attributes);
        VertexInputState(const VertexInputBinding& binding,
            const VertexInputAttribute& attribute);
        VertexInputState(const VertexInputBinding& binding,
            const std::vector<VertexInputAttribute>& attributes);  
        ~VertexInputState();
        MAGMA_TYPE_CAST(VkPipelineVertexInputStateCreateInfo, state)

    private:
        VkPipelineVertexInputStateCreateInfo state;
    };

    namespace states
    {
        extern const VertexInputState vertexHalf;
        extern const VertexInputState vertex2Half;
        extern const VertexInputState vertex3Half;
        extern const VertexInputState vertex4Half;

        extern const VertexInputState vertexFloat;
        extern const VertexInputState vertex2Float;
        extern const VertexInputState vertex3Float;
        extern const VertexInputState vertex4Float;
    } // namespace states
} // namespace magma
