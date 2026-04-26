namespace magma
{
template<class DescriptorSetTable>
inline VariableCountDescriptorSet::VariableCountDescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool,
    DescriptorSetTable& setTable, VkShaderStageFlags stageFlags,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    lent_ptr<IShaderReflectionFactory> shaderReflectionFactory /* nullptr */,
    std::string_view shaderFileName /* empty */,
    uint32_t setIndex /* 0 */,
    const StructureChain& extendedInfo /* default */):
    DescriptorSet(std::move(descriptorPool), std::move(allocator))
{   // Iterate over members of descriptor set table
    std::vector<uint32_t> locations;
    uint32_t variableSizedDescriptorCount = 0;
    uint32_t variableSizedDescriptorBinding = 0;
    boost::pfr::for_each_field(setTable,
        [&](auto& descriptor)
        {
            static_assert(std::is_base_of<DescriptorSetLayoutBinding, std::decay_t<decltype(descriptor)>>::value,
                "member of set table must have a base type of magma::DescriptorSetLayoutBinding");
            // Set global stage flags if they have not been assigned
            if (!descriptor.stageFlags)
                descriptor.stageFlags = stageFlags;
            VkDescriptorBindingFlagsEXT bindingFlags = descriptor.getBindingFlags();
            if (bindingFlags & VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT)
            {
                ++variableSizedDescriptorCount;
                variableSizedDescriptorBinding = descriptor.binding;
            }
            descriptors.push_back(&descriptor);
            locations.push_back(descriptor.binding);
        });
    std::sort(locations.begin(), locations.end());
    if (std::unique(locations.begin(), locations.end()) != locations.end())
        MAGMA_ERROR("elements of descriptor set layout should have unique binding locations");
    if (variableSizedDescriptorCount > 1)
        MAGMA_ERROR("descriptor set may have only one variable-sized descriptor");
    // This must only be used for the last binding in the descriptor set layout (i.e. the binding with the largest value of binding).
    if (variableSizedDescriptorBinding != locations.back())
        MAGMA_ERROR("variable-sized descriptor must be the member with the last binding");
    if (shaderReflectionFactory && !shaderFileName.empty())
    {   // Validate descriptors through shader reflection
        auto& shaderReflection = shaderReflectionFactory->getReflection(std::move(shaderFileName));
        validateReflection(shaderReflection, setIndex);
    }
    allocate(flags, extendedInfo);
    if (dirty())
        update();
}
} // namespace magma
