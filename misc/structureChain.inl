namespace magma
{
template<class StructureType>
inline StructureChain::Node::Node(const StructureType& node):
    size(sizeof(StructureType))
{
    static_assert(sizeof(StructureType) > sizeof(VkBaseInStructure),
        "chain structure size is too little");
    static_assert(std::is_trivially_copyable<StructureType>::value,
        "chain structure required to be trivially copyable");
    data = new uint8_t[size];
    memcpy(data, &node, size);
}

inline StructureChain::Node::Node(const Node& node):
    size(node.size)
{
    data = new uint8_t[size];
    memcpy(data, node.data, size);
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
    delete[] data;
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

#define MAGMA_SPECIALIZE_STRUCTURE_CHAIN_NODE(StructureType, structureType)\
template<>\
inline StructureType *magma::StructureChain::findNode<StructureType>() const noexcept\
{\
    auto it = std::find_if(chain.begin(), chain.end(),\
        [](auto& it)\
        {\
           return (it.getNode()->sType == structureType);\
        });\
    if (it != chain.end())\
        return reinterpret_cast<StructureType *>(it->getNode());\
    return nullptr;\
}
