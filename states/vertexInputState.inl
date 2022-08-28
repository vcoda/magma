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

constexpr VertexInputDivisorBinding::VertexInputDivisorBinding(const uint32_t binding, const uint32_t stride, const uint32_t divisor,
    const VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputBinding(binding, stride, inputRate),
    divisor(divisor)
{}

constexpr hash_t VertexInputDivisorBinding::hash() const noexcept
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
} // namespace magma
