namespace magma
{
template<std::size_t SpirvWordCount>
inline ShaderModule::ShaderModule(std::shared_ptr<Device> device, const SpirvWord (&bytecode)[SpirvWordCount],
    std::size_t bytecodeHash /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkShaderModuleCreateFlags flags /* 0 */,
    bool reflect /* false */
#ifdef VK_EXT_validation_cache
    ,std::shared_ptr<ValidationCache> validationCache /* nullptr */
#endif
    ):
    ShaderModule(std::move(device), bytecode, SpirvWordCount * sizeof(SpirvWord), bytecodeHash,
        std::move(allocator), flags, reflect
#ifdef VK_EXT_validation_cache
        ,std::move(validationCache)
#endif
    )
{}
} // namespace magma
