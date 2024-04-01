namespace magma
{
    /* Opaque blob of Vulkan structure that has at least
       sType and pNext members. */

    class StructureChain::Node
    {
    public:
        template<class StructureType>
        Node(const StructureType& blob) noexcept:
            size(sizeof(StructureType)),
            data(core::copyBinaryData(blob))
        {
            static_assert(sizeof(StructureType) > sizeof(VkBaseInStructure),
                "chain structure size is too little");
            static_assert(std::is_trivially_copyable<StructureType>::value,
                "chain structure required to be trivially copyable");
        }

        Node(const Node& node) noexcept:
            size(node.size),
            data(MAGMA_NEW uint8_t[node.size])
        {
            memcpy(data, node.data, node.size);
        }

        Node(Node&& node) noexcept:
            size(node.size),
            data(node.data)
        {
            node.size = 0;
            node.data = nullptr;
        }

        ~Node()
        {
            delete[] data;
        }

        VkBaseOutStructure *getNode() noexcept
        {
            return reinterpret_cast<VkBaseOutStructure *>(data);
        }

        const VkBaseInStructure *getNode() const noexcept
        {
            return reinterpret_cast<const VkBaseInStructure *>(data);
        }

        std::size_t getSize() const noexcept
        {
            return size;
        }

    private:
        std::size_t size;
        uint8_t *data;
    };

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
