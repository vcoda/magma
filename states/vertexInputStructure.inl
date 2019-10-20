namespace magma
{
template<typename Vertex, typename Type, bool unsignedNorm /* false */>
inline VertexInputAttribute::VertexInputAttribute(uint32_t location, Type Vertex::*attrib,
    normalized<unsignedNorm> /* Deduce <unsignedNorm> template parameter */) noexcept
{
    this->location = location;
    binding = 0;
    format = specialization::VertexAttribute<Type, unsignedNorm>().getFormat();
    ptrdiff_t diff = reinterpret_cast<ptrdiff_t>(&(((Vertex *)0)->*attrib));
    offset = static_cast<uint32_t>(diff);
}

template<typename Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const VertexInputAttribute& attribute,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */):
    VertexInputStructure<Vertex>(binding, {attribute}, inputRate)
{}

template<typename Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const std::initializer_list<VertexInputAttribute>& attributes,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */)
{
    VkVertexInputBindingDescription *vertexBindingDescription = new VkVertexInputBindingDescription[1];
    vertexBindingDescription->binding = binding;
    vertexBindingDescription->stride = sizeof(Vertex);
    vertexBindingDescription->inputRate = inputRate;
    VkVertexInputAttributeDescription *vertexAttributeDescriptions = internal::copyInitializerList(attributes);
    if (!vertexAttributeDescriptions)
        vertexAttributeDescriptionCount = 0;
    else
    {
        vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
        for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
            vertexAttributeDescriptions[i].binding = binding;
    }
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = vertexBindingDescription;
    pVertexAttributeDescriptions = vertexAttributeDescriptions;
}

template<typename Vertex>
template<uint32_t vertexAttributeCount>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const VertexInputAttribute(&attributes)[vertexAttributeCount],
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */)
{
    VkVertexInputBindingDescription *vertexBindingDescription = new VkVertexInputBindingDescription[1];
    vertexBindingDescription->binding = binding;
    vertexBindingDescription->stride = sizeof(Vertex);
    vertexBindingDescription->inputRate = inputRate;
    VkVertexInputAttributeDescription *vertexAttributeDescriptions = internal::copyArray(attributes, vertexAttributeCount);
    if (!vertexAttributeDescriptions)
        vertexAttributeDescriptionCount = 0;
    else
    {
        vertexAttributeDescriptionCount = vertexAttributeCount;
        for (uint32_t i = 0; i <  vertexAttributeDescriptionCount; ++i)
            vertexAttributeDescriptions[i].binding = binding;
    }
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = vertexBindingDescription;
    pVertexAttributeDescriptions = vertexAttributeDescriptions;
}

template<typename Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(const VertexInputStructure& other)
{
    sType = other.sType;
    pNext = other.pNext;
    flags = other.flags;
    vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
    pVertexBindingDescriptions = internal::copyArray(other.pVertexBindingDescriptions, 1);
    vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
    pVertexAttributeDescriptions = internal::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
}

template<typename Vertex>
inline VertexInputStructure<Vertex>& VertexInputStructure<Vertex>::operator=(const VertexInputStructure<Vertex>& other)
{
    if (this != &other)
    {
        sType = other.sType;
        pNext = other.pNext;
        flags = other.flags;
        vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
        delete[] pVertexBindingDescriptions;
        pVertexBindingDescriptions = internal::copyArray(other.pVertexBindingDescriptions, 1);
        vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
        delete[] pVertexAttributeDescriptions;
        pVertexAttributeDescriptions = internal::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
    }
    return *this;
}

template<typename Vertex>
inline uint32_t VertexInputStructure<Vertex>::stride(uint32_t binding) const noexcept
{
    MAGMA_ASSERT(0 == binding);
    if ((0 == vertexAttributeDescriptionCount) || (binding > 0))
        return 0;
    return static_cast<uint32_t>(sizeof(Vertex));
}
} // namespace magma
