namespace magma
{
constexpr VertexInputBinding::VertexInputBinding(const uint32_t binding, const uint32_t stride,
    const VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VkVertexInputBindingDescription{
        binding,
        stride,
        inputRate
    }
{}

constexpr hash_t VertexInputBinding::hash() const noexcept
{
    return core::hashArgs(
        binding,
        stride,
        inputRate);
}

constexpr VertexInputBindingDivisor::VertexInputBindingDivisor(const uint32_t binding, const uint32_t stride, const uint32_t divisor,
    const VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputBinding(binding, stride, inputRate),
    divisor(divisor)
{}

constexpr hash_t VertexInputBindingDivisor::hash() const noexcept
{
    return core::hashArgs(
        binding,
        stride,
        inputRate,
        divisor);
}

constexpr VertexInputAttribute::VertexInputAttribute(const uint32_t location, const uint32_t binding, const VkFormat format, const uint32_t offset) noexcept:
    VkVertexInputAttributeDescription{
        location,
        binding,
        format,
        offset
    }
{}

constexpr hash_t VertexInputAttribute::hash() const noexcept
{
    return core::hashArgs(
        location,
        binding,
        format,
        offset);
}

inline VertexInputState::VertexInputState() noexcept:
    VkPipelineVertexInputStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        nullptr,
        0, // flags
        0, // vertexBindingDescriptionCount
        nullptr, // pVertexBindingDescriptions
        0, // vertexAttributeDescriptionCount
        nullptr // pVertexAttributeDescriptions
    }
{
#ifdef VK_EXT_vertex_attribute_divisor
    vertexInputDivisorInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
    vertexInputDivisorInfo.pNext = nullptr;
    vertexInputDivisorInfo.vertexBindingDivisorCount = 0;
    vertexInputDivisorInfo.pVertexBindingDivisors = nullptr;
#endif // VK_EXT_vertex_attribute_divisor
}
} // namespace magma
