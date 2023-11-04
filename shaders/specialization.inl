namespace magma
{
template<class Block, class Type>
inline SpecializationEntry::SpecializationEntry(uint32_t constantID, Type Block::*member) noexcept:
    VkSpecializationMapEntry{
        constantID,
        static_cast<uint32_t>(reinterpret_cast<ptrdiff_t>(&(((Block*)0)->*member))), // offset
        sizeof(Type) // size
    }
{}

template<class Block>
inline Specialization::Specialization(const Block& data, const SpecializationEntry& entry) noexcept:
    VkSpecializationInfo{
        1, // mapEntryCount
        MAGMA_NEW SpecializationEntry[1], // pMapEntries
        sizeof(Block), // dataSize
        core::copyBinaryData(data) // pData
    }
{
    core::copy((VkSpecializationMapEntry *)pMapEntries, (VkSpecializationMapEntry *)&entry),
}

template<class Block>
inline Specialization::Specialization(const Block& data, const std::initializer_list<SpecializationEntry>& mapEntries) noexcept:
    VkSpecializationInfo{
        MAGMA_COUNT(mapEntries), // mapEntryCount
        core::copyInitializerList(mapEntries), // pMapEntries
        sizeof(Block),  // dataSize
        core::copyBinaryData(data) // pData
    }
{}
} // namespace magma
