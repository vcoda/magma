namespace magma
{
constexpr SubpassDependency::SubpassDependency() noexcept:
    VkSubpassDependency{0, 0, 0, 0, 0, 0, VK_DEPENDENCY_BY_REGION_BIT}
{}

constexpr SubpassDependency::SubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass,
    VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDependencyFlags dependencyFlags) noexcept:
    VkSubpassDependency{
        srcSubpass,
        dstSubpass,
        srcStageMask,
        dstStageMask,
        srcAccessMask,
        dstAccessMask,
        dependencyFlags
    }
{}

constexpr hash_t SubpassDependency::hash() const noexcept
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

constexpr SubpassDependency SubpassDependency::operator~() const noexcept
{
    return SubpassDependency(
        dstSubpass,
        srcSubpass,
        dstStageMask,
        srcStageMask,
        dstAccessMask,
        srcAccessMask,
        dependencyFlags);
}

constexpr SubpassExternalDependency::SubpassExternalDependency(
    VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDependencyFlags dependencyFlags /* VK_DEPENDENCY_BY_REGION_BIT */) noexcept:
    SubpassDependency(VK_SUBPASS_EXTERNAL, 0, srcStageMask, dstStageMask, srcAccessMask, dstAccessMask, dependencyFlags)
{}

constexpr SubpassSelfDependency::SubpassSelfDependency(uint32_t subpassIndex,
    VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDependencyFlags dependencyFlags /* VK_DEPENDENCY_BY_REGION_BIT */) noexcept:
    SubpassDependency(subpassIndex, subpassIndex, srcStageMask, dstStageMask, srcAccessMask, dstAccessMask, dependencyFlags)
{}
} // namespace magma
