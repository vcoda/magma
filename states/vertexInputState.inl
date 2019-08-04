namespace magma
{
constexpr VertexInputBinding::VertexInputBinding(uint32_t binding, uint32_t stride,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */)
{
    this->binding = binding;
    this->stride = stride;
    this->inputRate = inputRate;
}

constexpr VertexInputAttribute::VertexInputAttribute(uint32_t location,
    uint32_t binding, VkFormat format, uint32_t offset)
{
    this->location = location;
    this->binding = binding;
    this->format = format;
    this->offset = offset;
}

constexpr VertexInputAttribute::VertexInputAttribute(uint32_t location,
    const VertexInputAttribute& attrib, uint32_t offset)
{
    this->location = location;
    binding = attrib.binding;
    format = attrib.format;
    this->offset = offset;
}

constexpr VertexInputState::VertexInputState()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 0;
    pVertexBindingDescriptions = nullptr;
    vertexAttributeDescriptionCount = 0;
    pVertexAttributeDescriptions = nullptr;
}

constexpr VertexInputState::VertexInputState(const VertexInputAttribute& attribute)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 0;
    pVertexBindingDescriptions = nullptr;
    vertexAttributeDescriptionCount = 1;
    pVertexAttributeDescriptions = &attribute;
}

template<size_t VertexAttributeDescriptionCount>
constexpr VertexInputState::VertexInputState(const VertexInputAttribute (&attributes)[VertexAttributeDescriptionCount])
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 0;
    pVertexBindingDescriptions = nullptr;
    vertexAttributeDescriptionCount = VertexAttributeDescriptionCount;
    pVertexAttributeDescriptions = attributes;
}

constexpr VertexInputState::VertexInputState(const VertexInputBinding& binding, const VertexInputAttribute& attribute)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = &binding;
    vertexAttributeDescriptionCount = 1;
    pVertexAttributeDescriptions = &attribute;
}

template<size_t VertexAttributeDescriptionCount>
constexpr VertexInputState::VertexInputState(const VertexInputBinding& binding,
    const VertexInputAttribute (&attributes)[VertexAttributeDescriptionCount])
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = &binding;
    vertexAttributeDescriptionCount = VertexAttributeDescriptionCount;
    pVertexAttributeDescriptions = attributes;
}

template<size_t VertexBindingDescriptionCount, size_t VertexAttributeDescriptionCount>
constexpr VertexInputState::VertexInputState(const VertexInputBinding (&bindings)[VertexBindingDescriptionCount],
    const VertexInputAttribute (&attributes)[VertexAttributeDescriptionCount])
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = VertexBindingDescriptionCount;
    pVertexBindingDescriptions = bindings;
    vertexAttributeDescriptionCount = VertexAttributeDescriptionCount;
    pVertexAttributeDescriptions = attributes;
}

constexpr uint32_t VertexInputState::stride(uint32_t binding) const
{
    size_t stride = 0;
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
    {
        const VkVertexInputAttributeDescription& attrib = pVertexAttributeDescriptions[i];
        if (attrib.binding == binding)
        {
            const size_t attribSize = Format(attrib.format).size();
            stride += attribSize;
        }
    }
    return static_cast<uint32_t>(stride);
}

constexpr size_t VertexInputState::hash() const
{
    size_t hash = internal::hashArgs(
        flags,
        vertexBindingDescriptionCount,
        vertexAttributeDescriptionCount);
    for (uint32_t i = 0; i < vertexBindingDescriptionCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pVertexBindingDescriptions[i].binding,
            pVertexBindingDescriptions[i].stride,
            pVertexBindingDescriptions[i].inputRate));
    }
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pVertexAttributeDescriptions[i].location,
            pVertexAttributeDescriptions[i].binding,
            pVertexAttributeDescriptions[i].format,
            pVertexAttributeDescriptions[i].offset));
    }
    return hash;
}

inline bool VertexInputState::operator==(const VertexInputState& other) const noexcept
{
    bool equal = (flags == other.flags) &&
        (vertexBindingDescriptionCount == other.vertexBindingDescriptionCount) &&
        (vertexAttributeDescriptionCount == other.vertexAttributeDescriptionCount);
    if (equal)
    {
        for (uint32_t i = 0; i < vertexBindingDescriptionCount; ++i)
        {
            equal |= 
            (pVertexBindingDescriptions[i].binding == other.pVertexBindingDescriptions[i].binding) &&
            (pVertexBindingDescriptions[i].stride == other.pVertexBindingDescriptions[i].stride) &&
            (pVertexBindingDescriptions[i].inputRate == other.pVertexBindingDescriptions[i].inputRate);
        }
        for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
        {
            equal |=
            (pVertexAttributeDescriptions[i].location == other.pVertexAttributeDescriptions[i].location) &&
            (pVertexAttributeDescriptions[i].binding == other.pVertexAttributeDescriptions[i].binding) &&
            (pVertexAttributeDescriptions[i].format == other.pVertexAttributeDescriptions[i].format) &&
            (pVertexAttributeDescriptions[i].offset == other.pVertexAttributeDescriptions[i].offset);
        }
    }
    return equal;
}
} // namespace magma
