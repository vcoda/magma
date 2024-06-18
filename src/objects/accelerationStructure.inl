namespace magma
{
inline bool AccelerationStructure::serialize(void *data) const noexcept
{
    return copyToMemory(data, nullptr, VK_COPY_ACCELERATION_STRUCTURE_MODE_SERIALIZE_KHR);
}

inline bool AccelerationStructure::deserialize(const void *data) noexcept
{
    return copyFromMemory(data, nullptr, VK_COPY_ACCELERATION_STRUCTURE_MODE_DESERIALIZE_KHR);
}
} // namespace magma
