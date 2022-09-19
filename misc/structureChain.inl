namespace magma
{
struct StructureChain::Node
{
    template<class Structure>
    Node(const Structure& node);
    VkBaseOutStructure *getNode() noexcept;
    const hash_t hash;
    std::vector<uint8_t> blob;
};

template<class Structure>
inline StructureChain::Node::Node(const Structure& node):
    hash(core::hashArray(reinterpret_cast<const uint8_t *>(&node), sizeof(Structure))),
    blob(sizeof(Structure))
{
    memcpy(blob.data(), &node, sizeof(Structure));
}

inline VkBaseOutStructure *StructureChain::Node::getNode() noexcept
{
    return reinterpret_cast<VkBaseOutStructure *>(blob.data());
}

template<class Structure>
inline void StructureChain::addNode(const Structure& node)
{
    static_assert(sizeof(Structure) > sizeof(VkBaseInStructure), "structure size is too little");
    chain.emplace_back(node);
}
} // namespace magma
