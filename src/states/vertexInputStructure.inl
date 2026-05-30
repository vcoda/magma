namespace magma
{
template<class Vertex, std::size_t VertexAttributeCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure() noexcept:
    vertexInputAttributes{}
{}

template<class Vertex, std::size_t VertexAttributeCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure(uint32_t binding,
    const VertexInputAttribute& attribute,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputState(binding, sizeof(Vertex), inputRate),
    vertexInputAttributes{}
{
    static_assert(VertexAttributeCount == 1, "vertex input attribute count must be 1");
    vertexAttributeDescriptionCount = 1;
    vertexInputAttributes[0] = attribute;
    vertexInputAttributes[0].binding = binding;
    pVertexAttributeDescriptions = vertexInputAttributes;
}

template<class Vertex, std::size_t VertexAttributeCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure(uint32_t binding,
    const std::array<magma::VertexInputAttribute, VertexAttributeCount>& attributes,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputState(binding, sizeof(Vertex), inputRate),
    vertexInputAttributes{}
{
    vertexAttributeDescriptionCount = VertexAttributeCount;
    for (std::size_t i = 0; i < VertexAttributeCount; ++i)
    {
        vertexInputAttributes[i] = attributes[i];
        vertexInputAttributes[i].binding = binding;
    }
    pVertexAttributeDescriptions = vertexInputAttributes;
}

#ifdef VK_EXT_vertex_attribute_divisor
template<class Vertex, std::size_t VertexAttributeCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure(uint32_t binding,
    const VertexInputAttribute& attribute, uint32_t divisor) noexcept:
    VertexInputState(binding, sizeof(Vertex), divisor),
    vertexInputAttributes{}
{
    static_assert(VertexAttributeCount == 1, "vertex input attribute count must be 1");
    vertexAttributeDescriptionCount = 1;
    vertexInputAttributes[0] = attribute;
    vertexInputAttributes[0].binding = binding;
    pVertexAttributeDescriptions = vertexInputAttributes;
}

template<class Vertex, std::size_t VertexAttributeCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure(uint32_t binding,
    const std::array<magma::VertexInputAttribute, VertexAttributeCount>& attributes, uint32_t divisor) noexcept:
    VertexInputState(binding, sizeof(Vertex), divisor),
    vertexInputAttributes{}
{
    vertexAttributeDescriptionCount = VertexAttributeCount;
    for (std::size_t i = 0; i < VertexAttributeCount; ++i)
    {
        vertexInputAttributes[i] = attributes[i];
        vertexInputAttributes[i].binding = binding;
    }
    pVertexAttributeDescriptions = vertexInputAttributes;
}
#endif // VK_EXT_vertex_attribute_divisor
} // namespace magma
