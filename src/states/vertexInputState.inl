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
        specialization::VertexAttribute<Type>::format(),
        static_cast<uint32_t>(reinterpret_cast<ptrdiff_t>(&(((Vertex *)0)->*attribute)))
    }
{
    static_assert(sizeof(Type) != sizeof(uint16_t) * 3,
        "6-byte attribute types are not allowed");
}

constexpr uint32_t VertexInputAttribute::size() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R8_SNORM:
    case VK_FORMAT_R8_USCALED:
    case VK_FORMAT_R8_SSCALED:
    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R8_SINT:
        return 1u;
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R8G8_SNORM:
    case VK_FORMAT_R8G8_USCALED:
    case VK_FORMAT_R8G8_SSCALED:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16_SNORM:
    case VK_FORMAT_R16_USCALED:
    case VK_FORMAT_R16_SSCALED:
    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16_SFLOAT:
        return 2u;
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_R8G8B8A8_SNORM:
    case VK_FORMAT_R8G8B8A8_USCALED:
    case VK_FORMAT_R8G8B8A8_SSCALED:
    case VK_FORMAT_R8G8B8A8_UINT:
    case VK_FORMAT_R8G8B8A8_SINT:
    case VK_FORMAT_B8G8R8A8_UNORM:
    case VK_FORMAT_B8G8R8A8_SNORM:
    case VK_FORMAT_B8G8R8A8_USCALED:
    case VK_FORMAT_B8G8R8A8_SSCALED:
    case VK_FORMAT_B8G8R8A8_UINT:
    case VK_FORMAT_B8G8R8A8_SINT:
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
    case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_UINT_PACK32:
    case VK_FORMAT_A2R10G10B10_SINT_PACK32:
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_UINT_PACK32:
    case VK_FORMAT_A2B10G10R10_SINT_PACK32:
    case VK_FORMAT_R16G16_UNORM:
    case VK_FORMAT_R16G16_SNORM:
    case VK_FORMAT_R16G16_USCALED:
    case VK_FORMAT_R16G16_SSCALED:
    case VK_FORMAT_R16G16_UINT:
    case VK_FORMAT_R16G16_SINT:
    case VK_FORMAT_R16G16_SFLOAT:
    case VK_FORMAT_R32_UINT:
    case VK_FORMAT_R32_SINT:
    case VK_FORMAT_R32_SFLOAT:
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
        return 4u;
    case VK_FORMAT_R16G16B16_UNORM:
    case VK_FORMAT_R16G16B16_SNORM:
    case VK_FORMAT_R16G16B16_USCALED:
    case VK_FORMAT_R16G16B16_SSCALED:
        return 6u;
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_R16G16B16A16_SNORM:
    case VK_FORMAT_R16G16B16A16_USCALED:
    case VK_FORMAT_R16G16B16A16_SSCALED:
    case VK_FORMAT_R16G16B16A16_UINT:
    case VK_FORMAT_R16G16B16A16_SINT:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
    case VK_FORMAT_R32G32_UINT:
    case VK_FORMAT_R32G32_SINT:
    case VK_FORMAT_R32G32_SFLOAT:
    case VK_FORMAT_R64_UINT:
    case VK_FORMAT_R64_SINT:
    case VK_FORMAT_R64_SFLOAT:
        return 8u;
    case VK_FORMAT_R32G32B32_UINT:
    case VK_FORMAT_R32G32B32_SINT:
    case VK_FORMAT_R32G32B32_SFLOAT:
        return 12u;
    case VK_FORMAT_R32G32B32A32_UINT:
    case VK_FORMAT_R32G32B32A32_SINT:
    case VK_FORMAT_R32G32B32A32_SFLOAT:
    case VK_FORMAT_R64G64_UINT:
    case VK_FORMAT_R64G64_SINT:
    case VK_FORMAT_R64G64_SFLOAT:
        return 16u;
    case VK_FORMAT_R64G64B64_UINT:
    case VK_FORMAT_R64G64B64_SINT:
    case VK_FORMAT_R64G64B64_SFLOAT:
        return 24u;
    case VK_FORMAT_R64G64B64A64_UINT:
    case VK_FORMAT_R64G64B64A64_SINT:
    case VK_FORMAT_R64G64B64A64_SFLOAT:
        return 32u;
    default:
        return 0u;
    }
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
