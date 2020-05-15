namespace magma
{
template<std::size_t WordCount>
inline ShaderModule::ShaderModule(std::shared_ptr<Device> device, const SpirvWord (&bytecode)[WordCount],
    std::size_t bytecodeHash /* 0 */, VkShaderModuleCreateFlags flags /* 0 */,
    std::shared_ptr<ValidationCache> validationCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    ShaderModule(std::move(device), bytecode, WordCount * sizeof(SpirvWord), bytecodeHash,
        flags, std::move(validationCache), std::move(allocator))
{}
} // namespace magma
