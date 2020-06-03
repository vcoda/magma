namespace magma
{
template<typename Type, typename Func>
inline std::vector<const Type *> ShaderReflection::enumerateObjects(Func func, const char *message) const
{
    std::vector<const Type *> reflectionData;
    uint32_t count = 0;
    func(&module, &count, nullptr);
    if (count)
    {
        reflectionData.resize(count);
        const SpvReflectResult result = func(&module, &count, const_cast<Type **>(reflectionData.data()));
        MAGMA_THROW_REFLECTION_FAILURE(result, message)
    }
    return reflectionData;
}

template<typename Type, typename Func>
inline std::vector<const Type *> ShaderReflection::enumerateEntryPointObjects(Func func, const char *name, const char *message) const
{
    std::vector<const Type *> reflectionData;
    uint32_t count = 0;
    func(&module, name, &count, nullptr);
    if (count)
    {
        reflectionData.resize(count);
        const SpvReflectResult result = func(&module, name, &count, const_cast<Type **>(reflectionData.data()));
        MAGMA_THROW_REFLECTION_FAILURE(result, message)
    }
    return reflectionData;
}
} // namespace magma
