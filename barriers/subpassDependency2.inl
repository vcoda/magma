namespace magma
{
constexpr SubpassDependency2::SubpassDependency2() noexcept:
    VkSubpassDependency2KHR{VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2_KHR, nullptr,
        0, 0, 0, 0, 0, 0, VK_DEPENDENCY_BY_REGION_BIT, 0}
{}

constexpr SubpassDependency2::SubpassDependency2(uint32_t srcSubpass, uint32_t dstSubpass,
    VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDependencyFlags dependencyFlags, int32_t viewOffset) noexcept:
    VkSubpassDependency2KHR{
        VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2_KHR,
        nullptr,
        srcSubpass,
        dstSubpass,
        srcStageMask,
        dstStageMask,
        srcAccessMask,
        dstAccessMask,
        dependencyFlags,
        viewOffset
    }
{}

constexpr hash_t SubpassDependency2::hash() const noexcept
{
    return core::hashArgs(
        sType,
        srcSubpass,
        dstSubpass,
        srcStageMask,
        dstStageMask,
        srcAccessMask,
        dstAccessMask,
        dependencyFlags,
        viewOffset);
}

constexpr SubpassDependency2 SubpassDependency2::operator~() const noexcept
{
    return SubpassDependency2(
        dstSubpass,
        srcSubpass,
        dstStageMask,
        srcStageMask,
        dstAccessMask,
        srcAccessMask,
        dependencyFlags,
        viewOffset);
}

constexpr SubpassExternalDependency2::SubpassExternalDependency2(
    VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDependencyFlags dependencyFlags /* VK_DEPENDENCY_BY_REGION_BIT */,
    int32_t viewOffset /* 0 */) noexcept:
    SubpassDependency2(VK_SUBPASS_EXTERNAL, 0, srcStageMask, dstStageMask, srcAccessMask, dstAccessMask, dependencyFlags, viewOffset)
{}

constexpr SubpassSelfDependency2::SubpassSelfDependency2(uint32_t subpassIndex,
    VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDependencyFlags dependencyFlags /* VK_DEPENDENCY_BY_REGION_BIT */,
    int32_t viewOffset /* 0 */) noexcept:
    SubpassDependency2(subpassIndex, subpassIndex, srcStageMask, dstStageMask, srcAccessMask, dstAccessMask, dependencyFlags, viewOffset)
{}
} // namespace magma
