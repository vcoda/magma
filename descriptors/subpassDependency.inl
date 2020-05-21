namespace magma
{
constexpr SubpassDependency::SubpassDependency():
	VkSubpassDependency{}
{}

constexpr SubpassDependency::SubpassDependency(const uint32_t srcSubpass, const uint32_t dstSubpass,
    const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask,
    const VkDependencyFlags dependencyFlags /* VK_DEPENDENCY_BY_REGION_BIT */):
    VkSubpassDependency{
        srcSubpass,
        dstSubpass,
        srcStageMask,
        dstStageMask,
        0, // srcAccessMask
        0, // dstAccessMask
        dependencyFlags
    }
{
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
}

inline std::size_t SubpassDependency::hash() const noexcept
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
