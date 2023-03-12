namespace magma
{
namespace helpers
{
template<typename Type>
inline AlignedUniformArray<Type>::AlignedUniformArray(void *const buffer, const uint32_t arraySize, const VkDeviceSize alignment) noexcept:
    buffer(reinterpret_cast<char *const>(buffer)),
    arraySize(arraySize),
    alignment(alignment)
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(arraySize > 0);
    MAGMA_ASSERT(alignment > 0);
}

template<typename Type>
inline Type& AlignedUniformArray<Type>::operator[](const uint32_t index) noexcept
{
    MAGMA_ASSERT(index < arraySize);
    char *const elem = buffer + index * alignment;
    return *reinterpret_cast<Type *>(elem);
}

template<typename Type>
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
} // namespace helpers
} // namespace magma
