namespace magma
{
inline Resource::Payload::~Payload()
{
    free(data);
}

template<class Type>
inline void Resource::Payload::setData(const Type& payload) noexcept
{   // Objects of trivially-copyable types are the only C++ objects
    // that may be safely copied with std::memcpy().
    static_assert(std::is_trivially_copyable<Type>::value, "payload should be of trivially-copyable type");
    if (size < sizeof(Type))
    {
        free(data);
        data = malloc(sizeof(Type));
        size = sizeof(Type);
    }
    if (data)
        memcpy(data, &payload, sizeof(Type));
}

template<class Type>
inline Type& Resource::Payload::getData()
{
    static_assert(std::is_trivially_copyable<Type>::value, "payload should be of trivially-copyable type");
    MAGMA_ASSERT(data);
    if (!data || size < sizeof(Type))
        MAGMA_THROW(data ? "payload size it too little" : "payload not exists");
    Type& payload = *reinterpret_cast<Type *>(data);
    return payload;
}

inline void Resource::Payload::freeData() noexcept
{
    free(data);
    data = nullptr;
    size = 0;
}
} // namespace magma
