namespace magma
{
constexpr VertexInputAttribute::VertexInputAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset) noexcept:
    VkVertexInputAttributeDescription{
        location,
        binding,
        format,
        offset
    }
{}

template<class Vertex, class Type>
inline VertexInputAttribute::VertexInputAttribute(uint32_t location, uint32_t binding, Type Vertex::*attribute) noexcept:
    VkVertexInputAttributeDescription{
        location,
        binding,
        specialization::VertexAttribute<Type>::getFormat(),
        static_cast<uint32_t>(reinterpret_cast<ptrdiff_t>(&(((Vertex *)0)->*attribute)))
    }
{
    static_assert(sizeof(Type) != sizeof(uint16_t) * 3,
        "6-byte attribute types are not allowed");
}

constexpr hash_t VertexInputAttribute::hash() const noexcept
{
    return core::hashArgs(
        location,
        binding,
        format,
        offset);
}

constexpr VertexInputState::VertexInputState() noexcept:
    VkPipelineVertexInputStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        nullptr,
        0, // flags
        0, // vertexBindingDescriptionCount
        nullptr, // pVertexBindingDescriptions
        0, // vertexAttributeDescriptionCount
        nullptr // pVertexAttributeDescriptions
    },
    vertexBinding{
        0, // binding,
        0, // stride
        VK_VERTEX_INPUT_RATE_VERTEX
    }
#ifdef VK_EXT_vertex_attribute_divisor
   ,vertexBindingDivisor{
        0, // binding
        0  // divisor
    },
    vertexInputDivisorInfo{
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT,
        nullptr,
        0, // vertexBindingDivisorCount
        nullptr // pVertexBindingDivisors
    }
#endif // VK_EXT_vertex_attribute_divisor
{}

constexpr VertexInputState::VertexInputState(uint32_t binding, uint32_t stride,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VkPipelineVertexInputStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        nullptr,
        0, // flags
        1, // vertexBindingDescriptionCount
        &vertexBinding, // pVertexBindingDescriptions
        0, // vertexAttributeDescriptionCount
        nullptr // pVertexAttributeDescriptions
    },
    vertexBinding{
        binding,
        stride,
        inputRate
    }
#ifdef VK_EXT_vertex_attribute_divisor
   ,vertexBindingDivisor{
        0, // binding
        0  // divisor
    },
    vertexInputDivisorInfo{
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT,
        nullptr,
        0, // vertexBindingDivisorCount
        nullptr // pVertexBindingDivisors
    }
#endif // VK_EXT_vertex_attribute_divisor
{}

#ifdef VK_EXT_vertex_attribute_divisor
constexpr VertexInputState::VertexInputState(uint32_t binding, uint32_t stride, uint32_t divisor) noexcept:
    VkPipelineVertexInputStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        &vertexInputDivisorInfo,
        0, // flags
        1, // vertexBindingDescriptionCount
        &vertexBinding, // pVertexBindingDescriptions
        0, // vertexAttributeDescriptionCount
        nullptr // pVertexAttributeDescriptions
    },
    vertexBinding{
        binding,
        stride,
        VK_VERTEX_INPUT_RATE_INSTANCE
    },
    vertexBindingDivisor{
        binding,
        divisor
    },
    vertexInputDivisorInfo{
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT,
        nullptr,
        1, // vertexBindingDivisorCount
        &vertexBindingDivisor // pVertexBindingDivisors
    }
{}
#endif // VK_EXT_vertex_attribute_divisor

constexpr VertexInputState::VertexInputState(const VertexInputState& other) noexcept:
    VkPipelineVertexInputStateCreateInfo{
        other.sType,
    #ifdef VK_EXT_vertex_attribute_divisor
        (other.pNext == &other.vertexInputDivisorInfo) ?
            &vertexInputDivisorInfo : nullptr,
    #else
        nullptr,
    #endif
        other.flags,
        other.vertexBindingDescriptionCount,
        &vertexBinding,
        other.vertexAttributeDescriptionCount,
        other.pVertexAttributeDescriptions // Keep a reference until the derived class copies this array!
    },
    vertexBinding(other.vertexBinding)
#ifdef VK_EXT_vertex_attribute_divisor
   ,vertexBindingDivisor(other.vertexBindingDivisor),
    vertexInputDivisorInfo{
        other.vertexInputDivisorInfo.sType,
        nullptr,
        other.vertexInputDivisorInfo.vertexBindingDivisorCount,
        &vertexBindingDivisor // pVertexBindingDivisors
    }
#endif
{}

constexpr hash_t VertexInputState::hash() const noexcept
{
    hash_t hash = core::hashArgs(
        sType,
        flags,
        vertexBindingDescriptionCount,
        vertexAttributeDescriptionCount);
    for (uint32_t i = 0; i < vertexBindingDescriptionCount; ++i)
    {
        auto const& vertexBindingDescription = pVertexBindingDescriptions[i];
        hash = core::hashCombine(hash, core::hashArgs(
            vertexBindingDescription.binding,
            vertexBindingDescription.stride,
            vertexBindingDescription.inputRate));
    }
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
    {
        auto const& vertexAttribDescription = pVertexAttributeDescriptions[i];
        hash = core::hashCombine(hash, core::hashArgs(
            vertexAttribDescription.location,
            vertexAttribDescription.binding,
            vertexAttribDescription.format,
            vertexAttribDescription.offset));
    }
#ifdef VK_EXT_vertex_attribute_divisor
    if (pNext == &vertexInputDivisorInfo)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            vertexInputDivisorInfo.sType,
            vertexInputDivisorInfo.vertexBindingDivisorCount));
        for (uint32_t i = 0; i < vertexInputDivisorInfo.vertexBindingDivisorCount; ++i)
        {
            auto const& vertexBindingDivisor = vertexInputDivisorInfo.pVertexBindingDivisors[i];
            hash = core::hashCombine(hash, core::hashArgs(
                vertexBindingDivisor.binding,
                vertexBindingDivisor.divisor));
        }
    }
#endif // VK_EXT_vertex_attribute_divisor
    return hash;
}
} // namespace magma
