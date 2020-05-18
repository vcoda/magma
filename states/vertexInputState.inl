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

constexpr VertexInputAttribute::VertexInputAttribute(const uint32_t location, const uint32_t binding, const VkFormat format, const uint32_t offset) noexcept:
    VkVertexInputAttributeDescription{
        location,
        binding,
        format,
        offset
    }
{}
} // namespace magma
