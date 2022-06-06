namespace magma
{
template<typename Type>
inline void Resource::setPayload(const Type& data)
{   // Objects of trivially-copyable types are the only C++ objects
    // that may be safely copied with std::memcpy().
    static_assert(std::is_trivially_copyable<Type>::value, "payload should be of trivially-copyable type");
    if (payloadSize != sizeof(Type))
    {
        delete[] payload;
        payload = new uint8_t[sizeof(Type)];
        payloadSize = sizeof(Type);
    }
    memcpy(payload, &data, sizeof(Type));
}

template<typename Type>
inline Type& Resource::getPayload()
{
    static_assert(std::is_trivially_copyable<Type>::value, "payload should be of trivially-copyable type");
    MAGMA_ASSERT(payload);
    if (payloadSize < sizeof(Type))
        MAGMA_THROW("payload size not large enough");
    return *reinterpret_cast<Type *>(payload);
}
} // namespace magma
