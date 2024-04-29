namespace magma
{
inline StructureChain::StructureChain() noexcept:
    head(nullptr),
    hash(0ull)
{}

inline StructureChain::StructureChain(StructureChain&& other) noexcept:
    head(other.head), hash(other.hash)
{
    other.head = nullptr;
    other.hash = 0ull;
}

template<class StructureType>
inline void StructureChain::linkNode(const StructureType& node) noexcept
{
    static_assert(std::is_trivially_copyable<StructureType>::value,
        "node type is required to be trivially copyable");
    static_assert(sizeof(StructureType) > sizeof(VkBaseOutStructure),
        "node type size mismatch");
    MAGMA_ASSERT(sizeofNode(node.sType) == sizeof(StructureType));
    VkBaseOutStructure *tail = (VkBaseOutStructure *)malloc(sizeof(StructureType));
    MAGMA_ASSERT(tail);
    memcpy(tail, &node, sizeof(StructureType));
    if (!head)
        head = tail;
    else
    {
        VkBaseOutStructure *curr;
        for (curr = head; curr->pNext; curr = curr->pNext);
        curr->pNext = tail;
    }
    tail->pNext = nullptr;
    hash = core::hashCombine(hash, core::hashArray(
        (uint8_t *)tail, sizeof(StructureType)));
}

#define MAGMA_SPECIALIZE_STRUCTURE_CHAIN_NODE(StructureType, structureType)\
template<>\
inline StructureType *magma::StructureChain::findNode<StructureType>() noexcept\
{\
    return reinterpret_cast<StructureType *>(getNode(structureType));\
}\
\
template<>\
inline const StructureType *magma::StructureChain::findNode<StructureType>() const noexcept\
{\
    return reinterpret_cast<const StructureType *>(getNode(structureType));\
}
} // namespace magma
