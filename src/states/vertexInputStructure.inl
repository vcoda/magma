namespace magma
{
template<class Vertex>
inline VertexInputStructure<Vertex>::~VertexInputStructure()
{
    release();
}

template<class Vertex>
inline void VertexInputStructure<Vertex>::release()
{
    pVertexBindingDescriptions = nullptr; // delete[] pVertexBindingDescriptions in VertexInputState will be no-op
}

template<class Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const VertexInputAttribute& attribute,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept
{
    static_assert(sizeof(Vertex) % sizeof(uint32_t) == 0, "vertex structure size should be a multiple of 4 bytes");
    vertexBindingDescription.binding = binding;
    vertexBindingDescription.stride = sizeof(Vertex);
    vertexBindingDescription.inputRate = inputRate;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = &vertexBindingDescription;
    VkVertexInputAttributeDescription *vertexAttributeDescription = core::copyArray(&attribute, 1);
    MAGMA_ASSERT(vertexAttributeDescription);
    vertexAttributeDescription->binding = binding;
    vertexAttributeDescriptionCount = 1;
    pVertexAttributeDescriptions = vertexAttributeDescription;
}

template<class Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const std::initializer_list<VertexInputAttribute>& attributes,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept
{
    static_assert(sizeof(Vertex) % sizeof(uint32_t) == 0, "vertex structure size should be a multiple of 4 bytes");
    vertexBindingDescription.binding = binding;
    vertexBindingDescription.stride = sizeof(Vertex);
    vertexBindingDescription.inputRate = inputRate;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = &vertexBindingDescription;
    VkVertexInputAttributeDescription *vertexAttributeDescriptions = core::copyInitializerList(attributes);
    MAGMA_ASSERT(vertexAttributeDescriptions);
    vertexAttributeDescriptionCount = core::countof(attributes);
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
        vertexAttributeDescriptions[i].binding = binding;
    pVertexAttributeDescriptions = vertexAttributeDescriptions;
}

template<class Vertex>
template<uint32_t vertexAttributeCount>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const VertexInputAttribute(&attributes)[vertexAttributeCount],
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept
{
    static_assert(sizeof(Vertex) % sizeof(uint32_t) == 0, "vertex structure size should be a multiple of 4 bytes");
    vertexBindingDescription.binding = binding;
    vertexBindingDescription.stride = sizeof(Vertex);
    vertexBindingDescription.inputRate = inputRate;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = &vertexBindingDescription;
    VkVertexInputAttributeDescription *vertexAttributeDescriptions = core::copyArray(attributes, vertexAttributeCount);
    MAGMA_ASSERT(vertexAttributeDescriptions);
    vertexAttributeDescriptionCount = vertexAttributeCount;
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
        vertexAttributeDescriptions[i].binding = binding;
    pVertexAttributeDescriptions = vertexAttributeDescriptions;
}

template<class Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(const VertexInputStructure& other) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = other.pNext;
    flags = other.flags;
    vertexBindingDescription = other.vertexBindingDescription;
    vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
    pVertexBindingDescriptions = &vertexBindingDescription;
    vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
    pVertexAttributeDescriptions = core::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
}

template<class Vertex>
inline VertexInputStructure<Vertex>& VertexInputStructure<Vertex>::operator=(const VertexInputStructure<Vertex>& other) noexcept
{
    if (this != &other)
    {
        release();
        flags = other.flags;
        vertexBindingDescription = other.vertexBindingDescription;
        vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
        pVertexBindingDescriptions = &vertexBindingDescription;
        vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
        pVertexAttributeDescriptions = core::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
    }
    return *this;
}

template<class Vertex>
inline uint32_t VertexInputStructure<Vertex>::stride(uint32_t binding) const noexcept
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
