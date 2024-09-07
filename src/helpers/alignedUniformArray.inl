namespace magma::helpers
{
template<class Type>
inline AlignedUniformArray<Type>::AlignedUniformArray(void *const buffer, const uint32_t arraySize, const VkDeviceSize alignment) noexcept:
    buffer(reinterpret_cast<char *const>(buffer)),
    arraySize(arraySize),
    alignment(alignment),
    minIndex(std::numeric_limits<uint32_t>::max()),
    maxIndex(std::numeric_limits<uint32_t>::min())
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(arraySize > 0);
    MAGMA_ASSERT(alignment > 0);
}

template<class Type>
inline Type& AlignedUniformArray<Type>::operator[](const uint32_t index) noexcept
{
    MAGMA_ASSERT(index < arraySize);
    minIndex = std::min(minIndex, index);
    maxIndex = std::max(maxIndex, index);
    char *const elem = buffer + index * alignment;
    return *reinterpret_cast<Type *>(elem);
}

template<class Type>
class AlignedUniformArray<Type>::Iterator
{
public:
    explicit Iterator(char *ptr, const VkDeviceSize alignment) noexcept:
        ptr(ptr),
        alignment(alignment)
    {}

    Iterator& operator++() noexcept
    {
        ptr += alignment;
        return *this;
    }

    Iterator operator++(int) noexcept
    {
        Iterator temp = *this;
        ptr += alignment;
        return temp;
    }

    Iterator& operator--() noexcept
    {
        ptr -= alignment;
        return *this;
    }

    Iterator operator--(int) noexcept
    {
        Iterator temp = *this;
        ptr -= alignment;
        return temp;
    }

    bool operator!=(const Iterator& it) const noexcept
    {
        return ptr != it.ptr;
    }

    Type& operator*() noexcept
    {
        return *reinterpret_cast<Type *>(ptr);
    }

private:
    char *ptr;
    const VkDeviceSize alignment;
};
} // namespace magma::helpers
