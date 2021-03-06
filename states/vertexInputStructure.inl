namespace magma
{
template<typename Vertex, typename Type, bool normalized /* false */>
inline VertexInputAttribute::VertexInputAttribute(uint32_t location, Type Vertex::*attrib,
    Normalized<normalized> /* deduce template <normalized> parameter */) noexcept
{
    this->location = location;
    binding = 0;
    format = specialization::VertexAttribute<Type, normalized>().getFormat(); // constexpr value
    const ptrdiff_t diff = reinterpret_cast<ptrdiff_t>(&(((Vertex *)0)->*attrib));
    offset = static_cast<uint32_t>(diff);
}

template<typename Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const VertexInputAttribute& attribute,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */)
{
    VkVertexInputBindingDescription *vertexBindingDescription = new VkVertexInputBindingDescription[1];
    vertexBindingDescription->binding = binding;
    vertexBindingDescription->stride = sizeof(Vertex);
    vertexBindingDescription->inputRate = inputRate;
    VkVertexInputAttributeDescription *vertexAttributeDescription = core::copyArray(&attribute, 1);
    vertexAttributeDescription->binding = binding;
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = vertexBindingDescription;
    vertexAttributeDescriptionCount = 1;
    pVertexAttributeDescriptions = vertexAttributeDescription;
}

template<typename Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const std::initializer_list<VertexInputAttribute>& attributes,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */)
{
    VkVertexInputBindingDescription *vertexBindingDescription = new VkVertexInputBindingDescription[1];
    vertexBindingDescription->binding = binding;
    vertexBindingDescription->stride = sizeof(Vertex);
    vertexBindingDescription->inputRate = inputRate;
    VkVertexInputAttributeDescription *vertexAttributeDescriptions = core::copyInitializerList(attributes);
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
    VkVertexInputAttributeDescription *vertexAttributeDescriptions = core::copyArray(attributes, vertexAttributeCount);
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
inline VertexInputStructure<Vertex>::VertexInputStructure(const VertexInputStructure& other) noexcept
{
    sType = other.sType;
    pNext = other.pNext;
    flags = other.flags;
    vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
    pVertexBindingDescriptions = core::copyArray(other.pVertexBindingDescriptions, 1);
    vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
    pVertexAttributeDescriptions = core::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
}

template<typename Vertex>
inline VertexInputStructure<Vertex>& VertexInputStructure<Vertex>::operator=(const VertexInputStructure<Vertex>& other) noexcept
{
    if (this != &other)
    {
        sType = other.sType;
        pNext = other.pNext;
        flags = other.flags;
        vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
        delete[] pVertexBindingDescriptions;
        pVertexBindingDescriptions = core::copyArray(other.pVertexBindingDescriptions, 1);
        vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
        delete[] pVertexAttributeDescriptions;
        pVertexAttributeDescriptions = core::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
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

namespace specialization
{
template<typename Type, bool normalized>
constexpr VertexAttribute<Type, normalized>::VertexAttribute()
{
#ifdef _MSC_VER
    // TODO: By design this should be called in compile-time when there is no user-provided
    // specialization for concrete type, but this way it doesn't work on GCC.
    static_assert(false, "vertex attribute type not specialized");
#endif
}
} // namespace specialization
} // namespace magma
