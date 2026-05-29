namespace magma
{
template<class Vertex, std::size_t N>
inline VertexInputStructure<Vertex, N>::~VertexInputStructure()
{   // Make VertexInputState dtor a no-op
    pVertexBindingDescriptions = nullptr;
    pVertexAttributeDescriptions = nullptr;
}

template<class Vertex, std::size_t N>
inline VertexInputStructure<Vertex, N>::VertexInputStructure(uint32_t binding, VkVertexInputRate inputRate) noexcept:
    vertexInputBinding{
        binding,
        sizeof(Vertex),
        inputRate
    }
{
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = &vertexInputBinding;
}

template<class Vertex, std::size_t N>
inline VertexInputStructure<Vertex, N>::VertexInputStructure(uint32_t binding, const VertexInputAttribute& attribute,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputStructure<Vertex, N>(binding, inputRate)
{
    static_assert(N == 1, "vertex input attribute count must be 1");
    vertexInputAttributes[0] = attribute;
    vertexInputAttributes[0].binding = binding;
    vertexAttributeDescriptionCount = N;
    pVertexAttributeDescriptions = vertexInputAttributes;
}

template<class Vertex, std::size_t N>
inline VertexInputStructure<Vertex, N>::VertexInputStructure(uint32_t binding, const std::initializer_list<VertexInputAttribute>& attributes,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputStructure<Vertex, N>(binding, inputRate)
{
    MAGMA_ASSERT(attributes.size() == N);
    //vertexAttributeDescriptionCount = 0;
    for (auto const& attribute : attributes)
    {
        vertexInputAttributes[vertexAttributeDescriptionCount] = attribute;
        vertexInputAttributes[vertexAttributeDescriptionCount].binding = binding;
        ++vertexAttributeDescriptionCount;
    }
    pVertexAttributeDescriptions = vertexInputAttributes;
}

template<class Vertex, std::size_t N>
inline VertexInputStructure<Vertex, N>::VertexInputStructure(uint32_t binding, const VertexInputAttribute(&attributes)[N],
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept:
    VertexInputStructure<Vertex, N>(binding, inputRate)
{
    vertexAttributeDescriptionCount = N;
    for (std::size_t i = 0; i < N; ++i)
    {
        vertexInputAttributes[i] = attributes[i];
        vertexInputAttributes[i].binding = binding;
    }
    pVertexAttributeDescriptions = vertexInputAttributes;
}

template<class Vertex, std::size_t N>
inline VertexInputStructure<Vertex, N>::VertexInputStructure(const VertexInputStructure& other) noexcept:
    vertexInputBinding(other.vertexInputBinding)
{
    memcpy(vertexInputAttributes, other.vertexInputAttributes, N * sizeof(VkVertexInputAttributeDescription));
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = other.pNext;
    flags = other.flags;
    vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
    pVertexBindingDescriptions = &vertexInputBinding;
    vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
    pVertexAttributeDescriptions = vertexInputAttributes;
}

template<class Vertex, std::size_t N>
inline VertexInputStructure<Vertex, N>& VertexInputStructure<Vertex, N>::operator=(const VertexInputStructure<Vertex, N>& other) noexcept
{
    if (this != &other)
    {
        vertexInputBinding = other.vertexInputBinding;
        memcpy(vertexInputAttributes, other.vertexInputAttributes, N * sizeof(VkVertexInputAttributeDescription));
        pNext = other.pNext;
        flags = other.flags;
        vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
        pVertexBindingDescriptions = &vertexInputBinding;
        vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
        pVertexAttributeDescriptions = vertexInputAttributes;
    }
    return *this;
}

template<class Vertex, std::size_t N>
inline uint32_t VertexInputStructure<Vertex, N>::stride(uint32_t binding) const noexcept
{
    MAGMA_ASSERT(0 == binding);
    if ((0 == vertexAttributeDescriptionCount) || (binding > 0))
        return 0;
    return static_cast<uint32_t>(sizeof(Vertex));
}

template<class Vertex, class Type>
inline VertexInputAttribute::VertexInputAttribute(uint32_t location, Type Vertex::*attrib) noexcept:
    VkVertexInputAttributeDescription{
        location,
        0, // binding
        specialization::VertexAttribute<Type>::getFormat(),
        static_cast<uint32_t>(reinterpret_cast<ptrdiff_t>(&(((Vertex *)0)->*attrib)))
    }
{
    static_assert(sizeof(Type) != sizeof(uint16_t) * 3, "6-byte attribute types not allowed");
}
} // namespace magma
