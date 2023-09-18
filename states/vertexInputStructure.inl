namespace magma
{
template<class Vertex, class Type>
inline VertexInputAttribute::VertexInputAttribute(uint32_t location_, Type Vertex::*attrib) noexcept
{
    location = location_;
    binding = 0;
    format = specialization::VertexAttribute<Type>::format(); // constexpr value
    MAGMA_ASSERT(format != VK_FORMAT_UNDEFINED);
    const ptrdiff_t diff = reinterpret_cast<ptrdiff_t>(&(((Vertex *)0)->*attrib));
    offset = static_cast<uint32_t>(diff);
}

template<class Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const VertexInputAttribute& attribute,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */)
{
    static_assert(sizeof(Vertex) % sizeof(uint32_t) == 0, "vertex structure size should be a multiple of 4 bytes");
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

template<class Vertex>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const std::initializer_list<VertexInputAttribute>& attributes,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */)
{
    static_assert(sizeof(Vertex) % sizeof(uint32_t) == 0, "vertex structure size should be a multiple of 4 bytes");
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

template<class Vertex>
template<uint32_t vertexAttributeCount>
inline VertexInputStructure<Vertex>::VertexInputStructure(uint32_t binding, const VertexInputAttribute(&attributes)[vertexAttributeCount],
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */)
{
    static_assert(sizeof(Vertex) % sizeof(uint32_t) == 0, "vertex structure size should be a multiple of 4 bytes");
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

template<class Vertex>
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

template<class Vertex>
inline VertexInputStructure<Vertex>& VertexInputStructure<Vertex>::operator=(const VertexInputStructure<Vertex>& other) noexcept
{
    if (this != &other)
    {
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

template<class Vertex>
inline uint32_t VertexInputStructure<Vertex>::stride(uint32_t binding) const noexcept
{
    MAGMA_ASSERT(0 == binding);
    if ((0 == vertexAttributeDescriptionCount) || (binding > 0))
        return 0;
    return static_cast<uint32_t>(sizeof(Vertex));
}

namespace specialization
{
template<VkFormat Format, bool Unsigned, bool Normalized>
constexpr VkFormat VertexAttributeTraits<Format, Unsigned, Normalized>::format() noexcept
{
    static_assert(Format != VK_FORMAT_UNDEFINED, "vertex attribute type not specialized");
    return Format;
}
} // namespace specialization
} // namespace magma
