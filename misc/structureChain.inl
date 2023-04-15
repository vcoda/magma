namespace magma
{
struct StructureChain::Node
{
    template<class StructureType>
    Node(const StructureType& node)
    {
        data = new uint8_t[sizeof(StructureType)];
        memcpy(data, &node, sizeof(StructureType));
        hash = core::hashArray(data, sizeof(StructureType));
    }

    ~Node()
    {
        delete[] data;
    }

    VkBaseOutStructure *getNode() noexcept
    {
        return reinterpret_cast<VkBaseOutStructure *>(data);
    }

    uint8_t *data;
    hash_t hash;
};

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
