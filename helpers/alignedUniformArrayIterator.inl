namespace magma
{
namespace helpers
{
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
