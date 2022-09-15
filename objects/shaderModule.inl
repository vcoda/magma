namespace magma
{
template<std::size_t SpirvWordCount>
inline ShaderModule::ShaderModule(std::shared_ptr<Device> device, const SpirvWord (&bytecode)[SpirvWordCount],
    hash_t bytecodeHash /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool reflect /* false */,
    VkShaderModuleCreateFlags flags /* 0 */,
#ifdef VK_EXT_validation_cache
    std::shared_ptr<ValidationCache> validationCache /* nullptr */,
#endif
    const StructureChain& extendedInfo /* default */):
    ShaderModule(std::move(device), bytecode, SpirvWordCount * sizeof(SpirvWord), bytecodeHash, std::move(allocator), reflect, flags,
    #ifdef VK_EXT_validation_cache
        std::move(validationCache),
    #endif
        extendedInfo)
{}
} // namespace magma
