namespace magma
{
constexpr SubpassDependency::SubpassDependency()
{
    srcSubpass = 0;
    dstSubpass = 0;
    srcStageMask = 0;
    dstStageMask = 0;
    srcAccessMask = 0;
    dstAccessMask = 0;
    dependencyFlags = 0;
}

constexpr SubpassDependency::SubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass,
    VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkDependencyFlags dependencyFlags /* VK_DEPENDENCY_BY_REGION_BIT */)
{
    this->srcSubpass = srcSubpass;
    this->dstSubpass = dstSubpass;
    this->srcStageMask = srcStageMask;
    this->dstStageMask = dstStageMask;
    switch (srcStageMask)
    {
    case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
        srcAccessMask = VK_ACCESS_SHADER_READ_BIT; // ->Read
        break;
    case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
        srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; // ->Write
        break;
    }
    switch (dstStageMask)
    {
    case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
        dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; // Write->
        break;
    case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
        dstAccessMask = VK_ACCESS_SHADER_READ_BIT; // ->Read
        break;
    }
    this->dependencyFlags = dependencyFlags;
}

inline std::size_t SubpassDependency::hash() const
{
    return core::hashArgs(
        srcSubpass,
        dstSubpass,
        srcStageMask,
        dstStageMask,
        srcAccessMask,
        dstAccessMask,
        dependencyFlags);
}
} // namespace magma
