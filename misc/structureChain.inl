namespace magma
{
struct StructureChain::Node
{
    template<class StructureType>
    Node(const StructureType& node);
    VkBaseOutStructure *getNode() noexcept;
    const hash_t hash;
    std::vector<uint8_t> blob;
};

template<class StructureType>
inline StructureChain::Node::Node(const StructureType& node):
    hash(core::hashArray(reinterpret_cast<const uint8_t *>(&node), sizeof(StructureType))),
    blob(sizeof(StructureType))
{
    memcpy(blob.data(), &node, sizeof(StructureType));
}

inline VkBaseOutStructure *StructureChain::Node::getNode() noexcept
{
    return reinterpret_cast<VkBaseOutStructure *>(blob.data());
}

template<class StructureType>
inline void StructureChain::addNode(const StructureType& node)
{
    static_assert(sizeof(StructureType) > sizeof(VkBaseInStructure), "structure size is too little");
    chain.emplace_back(node);
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
