namespace magma
{
template<typename Type>
inline void Resource::setPayload(const Type& data)
{
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
    if (!payload)
        MAGMA_THROW("payload has not been assigned");
    if (payloadSize < sizeof(Type))
        MAGMA_THROW("payload size not large enough");
    return *reinterpret_cast<Type *>(payload);
}
} // namespace magma
