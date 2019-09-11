namespace magma
{
constexpr VertexInputBinding::VertexInputBinding(uint32_t binding, uint32_t stride,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */):
    VkVertexInputBindingDescription{}
{
    this->binding = binding;
    this->stride = stride;
    this->inputRate = inputRate;
}

constexpr VertexInputAttribute::VertexInputAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset):
    VkVertexInputAttributeDescription{}
{
    this->location = location;
    this->binding = binding;
    this->format = format;
    this->offset = offset;
}
} // namespace magma