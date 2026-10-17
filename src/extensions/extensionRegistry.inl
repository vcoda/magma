namespace magma
{
template<class Properties>
inline ExtensionRegistry<Properties>::ExtensionRegistry(std::vector<Properties> properties):
    registry(std::move(properties))
{
    names.reserve(registry.size());
    for (const auto& property: registry)
    {
        if constexpr (std::is_same_v<Properties, VkExtensionProperties>)
            names.push_back(property.extensionName);
        else if constexpr (std::is_same_v<Properties, VkLayerProperties>)
            names.push_back(property.layerName);
    }
    std::sort(names.begin(), names.end(),
        [](const char *lhs, const char *rhs)
        {
            return std::strcmp(lhs, rhs) < 0;
        });
}

template<class Properties>
inline bool ExtensionRegistry<Properties>::available(const char *extensionName) noexcept
{
    return std::binary_search(names.begin(), names.end(), extensionName,
        [](const char *lhs, const char *rhs)
        {
            return std::strcmp(lhs, rhs) < 0;
        });
}
} // namespace magma
