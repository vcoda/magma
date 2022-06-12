namespace magma
{
template<typename Type>
inline void Resource::setPayload(const Type& data)
{   // Objects of trivially-copyable types are the only C++ objects
    // that may be safely copied with std::memcpy().
    static_assert(std::is_trivially_copyable<Type>::value, "payload should be of trivially-copyable type");
    if (!payload)
        payload = new Payload;
    if (payload->size < sizeof(Type))
    {
        delete[] payload->data;
        payload->data = new uint8_t[sizeof(Type)];
    }
    memcpy(payload->data, &data, sizeof(Type));
    payload->size = sizeof(Type);
}

template<typename Type>
inline Type& Resource::getPayload()
{
    static_assert(std::is_trivially_copyable<Type>::value, "payload should be of trivially-copyable type");
    MAGMA_ASSERT(payload);
    MAGMA_ASSERT(payload->data);
    if (payload->size < sizeof(Type))
        MAGMA_THROW("payload size not large enough");
    Type& data = *reinterpret_cast<Type *>(payload->data);
    return data;
}
} // namespace magma
