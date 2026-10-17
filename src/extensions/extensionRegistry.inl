namespace magma
{
template<class Properties>
inline ExtensionRegistry<Properties>::ExtensionRegistry(const std::vector<Properties>& properties)
{
    hashes.reserve(properties.size());
    for (auto& property: properties)
    {
        if constexpr (std::is_same_v<Properties, VkExtensionProperties>)
            hashes.push_back(core::hashString(property.extensionName));
        else if constexpr (std::is_same_v<Properties, VkLayerProperties>)
            hashes.push_back(core::hashString(property.layerName));
    }
    std::sort(hashes.begin(), hashes.end());
}

template<class Properties>
inline bool ExtensionRegistry<Properties>::available(hash_t hash) const noexcept
{
    return std::binary_search(hashes.begin(), hashes.end(), hash);
}
} // namespace magma
