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
    core::copy(const_cast<VkSpecializationMapEntry *>(pMapEntries), static_cast<const VkSpecializationMapEntry *>(&entry));
}

template<class Block>
inline Specialization::Specialization(const Block& data, const std::initializer_list<SpecializationEntry>& mapEntries) noexcept:
    VkSpecializationInfo{
        core::countof(mapEntries), // mapEntryCount
        core::copyInitializerList(mapEntries), // pMapEntries
        sizeof(Block),  // dataSize
        core::copyBinaryData(data) // pData
    }
{}

inline Specialization::Specialization(Specialization&& other) noexcept:
    VkSpecializationInfo{
        other.mapEntryCount,
        other.pMapEntries,
        other.dataSize,
        other.pData
    }
{
    other.mapEntryCount = 0;
    other.pMapEntries = nullptr;
    other.dataSize = 0;
    other.pData = nullptr;
}

inline Specialization& Specialization::operator=(Specialization&& other) noexcept
{
    if (this != &other)
    {
        this->~Specialization();
        mapEntryCount = other.mapEntryCount;
        pMapEntries = other.pMapEntries;
        dataSize = other.dataSize;
        pData = other.pData;
        other.mapEntryCount = 0;
        other.pMapEntries = nullptr;
        other.dataSize = 0;
        other.pData = nullptr;
    }
    return *this;
}
} // namespace magma
