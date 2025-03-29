namespace magma
{
template<class Properties>
inline ExtensionRegistry<Properties>::ExtensionRegistry(const std::vector<Properties>& properties):
    count(0),
    supportedCount(0)
{
    for (auto const& property: properties)
    {
        if constexpr (std::is_same<Properties, VkExtensionProperties>::value)
            registry.emplace(property.extensionName, property);
        else if constexpr (std::is_same<Properties, VkLayerProperties>::value)
            registry.emplace(property.layerName, property);
    }
}

template<class Properties>
inline bool ExtensionRegistry<Properties>::supported(const char *name) noexcept
{
    ++count;
    if (registry.find(name) == registry.end())
        return false;
    ++supportedCount;
    return true;
}
} // namespace magma
