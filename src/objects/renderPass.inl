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
} // namespace magma
