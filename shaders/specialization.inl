namespace magma
{
template<class Block, class Type>
inline SpecializationEntry::SpecializationEntry(uint32_t index, Type Block::*member) noexcept
{
    constantID = index;
    const ptrdiff_t diff = reinterpret_cast<ptrdiff_t>(&(((Block*)0)->*member));
    offset = static_cast<uint32_t>(diff);
    size = sizeof(Type);
}

template<class Block>
inline Specialization::Specialization(const Block& data, const SpecializationEntry& entry) noexcept
{
    mapEntryCount = 1;
    pMapEntries = new(std::nothrow) SpecializationEntry[1];
    core::copy((VkSpecializationMapEntry *)pMapEntries, (VkSpecializationMapEntry *)&entry);
    dataSize = sizeof(Block);
    pData = core::copyArray<char>(&data, dataSize);
}

template<class Block>
inline Specialization::Specialization(const Block& data, const std::initializer_list<SpecializationEntry>& mapEntries) noexcept
{
    mapEntryCount = MAGMA_COUNT(mapEntries);
    pMapEntries = core::copyInitializerList(mapEntries);
    dataSize = sizeof(Block);
    pData = core::copyArray<char>(&data, dataSize);
}
} // namespace magma
