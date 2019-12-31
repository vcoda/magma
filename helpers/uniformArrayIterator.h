namespace magma
{
namespace helpers
{
template<typename Type>
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
