namespace magma
{
template<typename Type>
inline PipelineLayout::PipelineLayout(std::shared_ptr<Device> device,
    const pushconstants::PushConstantRange<Type>& pushConstantRange,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_LAYOUT, std::move(device), std::move(allocator))
{
    VkPipelineLayoutCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.setLayoutCount = 0;
    info.pSetLayouts = nullptr;
    info.pushConstantRangeCount = 1;
    info.pPushConstantRanges = &pushConstantRange;
    const VkResult create = vkCreatePipelineLayout(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create pipeline layout");
    hash = detail::hashArgs(
        info.sType,
        info.flags,
        info.setLayoutCount,
        info.pushConstantRangeCount);
    detail::hashCombine(hash, pushConstantRange.hash());
}
} // namespace magma
