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

#ifdef VK_EXT_vertex_attribute_divisor
constexpr VertexInputBindingDivisor::VertexInputBindingDivisor(const uint32_t binding, const uint32_t divisor):
    VkVertexInputBindingDivisorDescriptionEXT{
        binding,
        divisor
    }
{}

constexpr hash_t VertexInputBindingDivisor::hash() const noexcept
{
    return core::hashArgs(
        binding,
        divisor);
}
#endif // VK_EXT_vertex_attribute_divisor

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
