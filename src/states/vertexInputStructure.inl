namespace magma
{
template<class Vertex, std::size_t VertexAttributeCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure() noexcept:
    vertexBindings{},
    vertexInputAttributes{}
{}

template<class Vertex, std::size_t VertexAttributeCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure(uint32_t binding,
    const VertexInputAttribute& attribute,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputState(binding, sizeof(Vertex), inputRate),
    vertexBindings{},
    vertexInputAttributes{}
{
    static_assert(VertexAttributeCount == 1, "vertex input attribute count must be 1");
    vertexAttributeDescriptionCount = 1;
    vertexInputAttributes[0] = attribute;
    vertexInputAttributes[0].binding = binding;
    pVertexAttributeDescriptions = vertexInputAttributes;
}

namespace detail
{
template<std::size_t N>
static constexpr bool isUniqueBinding(uint32_t binding, std::size_t maxIndex,
    const std::array<VertexInputAttribute, N>& attributes) noexcept
{
    for (std::size_t i = 0; i < maxIndex; ++i)
        if (attributes[i].binding == binding)
            return false;
    return true;
}
} // namespace detail

template<class Vertex, std::size_t VertexAttributeCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure(
    const std::array<VertexInputAttribute, VertexAttributeCount>& attributes,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputState(0, sizeof(Vertex), inputRate),
    vertexBindings{},
    vertexInputAttributes{}
{
    uint32_t offsets[VertexAttributeCount] = {};
    for (std::size_t i = 0; i < VertexAttributeCount; ++i)
    {
        if (attributes[i].offset)
            offsets[i] = attributes[i].offset;
        else
        {   // Calculate offset of this attribute manually
            uint32_t offset = 0;
            for (std::size_t j = 0; j < i; ++j)
            {
                if (attributes[j].binding == attributes[i].binding)
                    offset += attributes[j].size(); // TODO: alignment
            }
            offsets[i] = offset;
        }
    }
    vertexBindingDescriptionCount = 0;
    for (std::size_t i = 0; i < VertexAttributeCount; ++i)
    {
        vertexInputAttributes[i] = attributes[i];
        vertexInputAttributes[i].offset = offsets[i];
        uint32_t binding = attributes[i].binding;
        if (detail::isUniqueBinding(binding, i, attributes))
        {   // Calculate stride inside this vertex stream
            uint32_t stride = 0;
            for (std::size_t j = 0; j < VertexAttributeCount; ++j)
            {
                if (attributes[j].binding == binding)
                {
                    uint32_t offset = offsets[j];
                    uint32_t size = attributes[j].size();
                    stride = std::max(stride, offset + size);
                }
            }
            vertexBindings[vertexBindingDescriptionCount].binding = binding;
            vertexBindings[vertexBindingDescriptionCount].stride = stride;
            vertexBindings[vertexBindingDescriptionCount].inputRate = inputRate;
            ++vertexBindingDescriptionCount;
        }
    }
    pVertexBindingDescriptions = vertexBindings;
    vertexAttributeDescriptionCount = VertexAttributeCount;
    pVertexAttributeDescriptions = vertexInputAttributes;
}

template<class Vertex, std::size_t VertexAttributeCount>
template<std::size_t VertexInputBindingCount>
constexpr VertexInputStructure<Vertex, VertexAttributeCount>::VertexInputStructure(
    const std::array<VertexInputAttribute, VertexAttributeCount>& attributes,
    const std::array<VkVertexInputRate, VertexInputBindingCount>& inputRates) noexcept:
    VertexInputStructure(attributes)
{
    for (std::size_t i = 0; i < VertexAttributeCount; ++i)
    {
        uint32_t binding = attributes[i].binding;
        if (detail::isUniqueBinding(binding, i, attributes))
        {
            for (uint32_t j = 0; j < vertexBindingDescriptionCount; ++j)
            {
                if (binding == vertexBindings[j].binding)
                {   // Define vertex input rate per binding
                    assert(binding < VertexInputBindingCount);
                    vertexBindings[j].inputRate = inputRates[binding];
                    break;
                }
            }
        }
    }
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
