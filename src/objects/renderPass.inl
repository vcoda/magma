namespace magma
{
inline RenderPass::RenderPass(std::shared_ptr<Device> device, const AttachmentDescription& attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkRenderPassCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), std::vector<AttachmentDescription>{attachment}, std::move(allocator), flags, extendedInfo)
{}

inline RenderPass::RenderPass(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator,
    const std::vector<AttachmentDescription>& attachments, VkRenderPassCreateFlags flags):
    NonDispatchable(VK_OBJECT_TYPE_RENDER_PASS, std::move(device), std::move(allocator)),
    flags(flags),
    attachments(attachments),
    hash(0ull)
{}

inline void RenderPass::begin(const std::vector<ImageView *>& imageViews) const noexcept
{
    for (size_t i = 0, n = std::min(attachments.size(), imageViews.size()); i < n; ++i)
        setInitialLayout(imageViews[i], attachments[i]);
}

inline void RenderPass::end(const std::vector<ImageView *>& imageViews) const noexcept
{
    for (size_t i = 0, n = std::min(attachments.size(), imageViews.size()); i < n; ++i)
        setFinalLayout(imageViews[i], attachments[i]);
}

inline void RenderPass::begin(const std::vector<std::shared_ptr<ImageView>>& imageViews) const noexcept
{
    for (size_t i = 0, n = std::min(attachments.size(), imageViews.size()); i < n; ++i)
        setInitialLayout(imageViews[i].get(), attachments[i]);
}

inline void RenderPass::end(const std::vector<std::shared_ptr<ImageView>>& imageViews) const noexcept
{
    for (size_t i = 0, n = std::min(attachments.size(), imageViews.size()); i < n; ++i)
        setFinalLayout(imageViews[i].get(), attachments[i]);
}
} // namespace magma
