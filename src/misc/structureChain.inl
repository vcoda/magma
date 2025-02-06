namespace magma
{
inline StructureChain::StructureChain() noexcept:
    head(nullptr)
{}

template<class StructureType>
inline StructureChain::StructureChain(const StructureType& node) noexcept:
    head(nullptr)
{
    linkNode(node);
}

inline StructureChain::StructureChain(StructureChain&& other) noexcept:
    head(other.head)
{
    other.head = nullptr;
}

template<class StructureType>
inline void StructureChain::linkNode(const StructureType& node) noexcept
{
    static_assert(std::is_trivially_copyable<StructureType>::value,
        "node type is required to be trivially copyable");
    static_assert(sizeof(StructureType) >= sizeof(VkBaseOutStructure),
        "node type size mismatch");
    MAGMA_ASSERT(getNodeSize(node.sType) == sizeof(StructureType));
    VkBaseOutStructure *tail = copyNode(reinterpret_cast<const VkBaseOutStructure *>(&node));
    if (!head)
        head = tail;
    else
        tailNode()->pNext = tail;
}

inline VkBaseOutStructure *StructureChain::tailNode() noexcept
{
    for (auto node = head; node; node = node->pNext)
    {
        if (!node->pNext)
            return node;
    }
    return nullptr;
}

inline const VkBaseInStructure *StructureChain::tailNode() const noexcept
{
    for (auto node = head; node; node = node->pNext)
    {
        if (!node->pNext)
            return reinterpret_cast<const VkBaseInStructure *>(node);
    }
    return nullptr;
}

inline size_t StructureChain::size() const noexcept
{
    size_t size = 0;
    for (auto node = head; node; node = node->pNext)
        ++size;
    return size;
}

inline hash_t StructureChain::getNodeHash(VkBaseOutStructure *node) const noexcept
{
    auto nextNode = node->pNext;
    node->pNext = nullptr; // Should not affect hash
    size_t size = getNodeSize(node->sType);
    hash_t hash = core::hashArray(reinterpret_cast<const uint8_t *>(node), size);
    node->pNext = nextNode;
    return hash;
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
