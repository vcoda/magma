namespace magma
{
namespace helpers
{
template<class Type>
inline UniformArray<Type>::UniformArray(void *const buffer, const uint32_t arraySize) noexcept:
    buffer(reinterpret_cast<Type *const>(buffer)),
    arraySize(arraySize),
    minIndex(std::numeric_limits<uint32_t>::max()),
    maxIndex(std::numeric_limits<uint32_t>::min())
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(arraySize > 0);
}

template<class Type>
inline Type& UniformArray<Type>::operator[](const uint32_t index) noexcept
{
    MAGMA_ASSERT(index < arraySize);
    minIndex = std::min(minIndex, index);
    maxIndex = std::max(maxIndex, index);
    return buffer[index];
}

template<class Type>
class UniformArray<Type>::Iterator
{
public:
    explicit Iterator(Type *ptr) noexcept:
        ptr(ptr)
    {}

    Iterator& operator++() noexcept
    {
        ++ptr;
        return *this;
    }

    Iterator operator++(int) noexcept
    {
        Iterator temp = *this;
        ++ptr;
        return temp;
    }

    Iterator& operator--() noexcept
    {
        --ptr;
        return *this;
    }

    Iterator operator--(int) noexcept
    {
        Iterator temp = *this;
        --ptr;
        return temp;
    }

    bool operator!=(const Iterator& it) const noexcept
    {
        return ptr != it.ptr;
    }

    Type& operator*() noexcept
    {
        return *ptr;
    }

private:
    Type *ptr;
};
} // namespace helpers
} // namespace magma
