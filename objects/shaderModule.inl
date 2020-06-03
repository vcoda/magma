namespace magma
{
template<std::size_t WordCount>
inline ShaderModule::ShaderModule(std::shared_ptr<Device> device, const SpirvWord (&bytecode)[WordCount],
    std::size_t bytecodeHash /* 0 */, VkShaderModuleCreateFlags flags /* 0 */, bool reflect /* false */,
    std::shared_ptr<IAllocator> allocator /* nullptr */
#ifdef VK_EXT_validation_cache
    ,std::shared_ptr<ValidationCache> validationCache /* nullptr */
#endif
    ):
    ShaderModule(std::move(device), bytecode, WordCount * sizeof(SpirvWord), bytecodeHash,
        flags, reflect, std::move(allocator)
#ifdef VK_EXT_validation_cache
        ,std::move(validationCache)
#endif
    )
{}
} // namespace magma
