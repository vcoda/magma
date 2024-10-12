namespace magma
{
inline PipelineLayout::PipelineLayout(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    PipelineLayout(std::move(device), {/* empty */}, std::move(allocator), flags)
{}

inline PipelineLayout::PipelineLayout(std::shared_ptr<const DescriptorSetLayout> setLayout,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    PipelineLayout(std::move(setLayout), {/* empty */}, std::move(allocator), flags)
{}

inline PipelineLayout::PipelineLayout(std::shared_ptr<Device> device, const PushConstantRange& pushConstantRange,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    PipelineLayout(std::move(device), {pushConstantRange}, std::move(allocator), flags)
{}

inline PipelineLayout::PipelineLayout(std::shared_ptr<const DescriptorSetLayout> setLayout,
    const PushConstantRange& pushConstantRange,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    PipelineLayout(std::move(setLayout), {pushConstantRange}, std::move(allocator), flags)
{}

inline PipelineLayout::PipelineLayout(const std::initializer_list<std::shared_ptr<const DescriptorSetLayout>>& setLayouts,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    PipelineLayout(setLayouts, {/* empty */}, std::move(allocator), flags)
{}

inline PipelineLayout::PipelineLayout(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator,
    VkPipelineLayoutCreateFlags flags, VkPipelineLayout layout):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_LAYOUT, std::move(device), std::move(allocator)),
    flags(flags)
{
    handle = layout;
}
} // namespace magma
