namespace magma
{
template<class StructureType>
inline StructureChain::Node::Node(const StructureType& node) noexcept:
    size(sizeof(StructureType))
{
    static_assert(sizeof(StructureType) > sizeof(VkBaseInStructure),
        "chain structure size is too little");
    static_assert(std::is_trivially_copyable<StructureType>::value,
        "chain structure required to be trivially copyable");
    data = core::copyBinaryData(node);
}

inline StructureChain::Node::Node(const Node& node) noexcept:
    size(node.size),
    data(MAGMA_NEW char[node.size])
{
    memcpy(data, node.data, node.size);
}

inline StructureChain::Node::Node(Node&& node) noexcept:
    size(node.size),
    data(node.data)
{
    node.size = 0;
    node.data = nullptr;
}

inline StructureChain::Node::~Node()
{
    delete[] reinterpret_cast<char *>(data);
}

inline VkBaseOutStructure *StructureChain::Node::getNode() noexcept
{
    return reinterpret_cast<VkBaseOutStructure *>(data);
}

inline const VkBaseInStructure *StructureChain::Node::getNode() const noexcept
{
    return reinterpret_cast<const VkBaseInStructure *>(data);
}

template<class StructureType>
inline void StructureChain::addNode(const StructureType& node)
{
    chain.emplace_back(node);
}

inline VkBaseOutStructure *StructureChain::firstNode() noexcept
{
    return chain.empty() ? nullptr : chain.begin()->getNode();
}

inline const VkBaseInStructure *StructureChain::firstNode() const noexcept
{
    return chain.empty() ? nullptr : chain.cbegin()->getNode();
}

inline VkBaseOutStructure *StructureChain::lastNode() noexcept
{
    return chain.empty() ? nullptr : chain.rbegin()->getNode();
}

inline const VkBaseInStructure *StructureChain::lastNode() const noexcept
{
    return chain.empty() ? nullptr : chain.crbegin()->getNode();
}
} // namespace magma
