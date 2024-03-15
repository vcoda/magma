namespace magma
{
inline RenderPass::RenderPass(std::shared_ptr<Device> device, const AttachmentDescription& attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */, const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), std::vector<AttachmentDescription>{attachment}, std::move(allocator), extendedInfo)
{}

inline RenderPass::RenderPass(std::shared_ptr<Device> device, const std::vector<AttachmentDescription>& attachments,
    const std::vector<SubpassDescription>& subpasses,  std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), attachments, subpasses, { /* subpasses */}, std::move(allocator), extendedInfo)
{}

inline RenderPass::RenderPass(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator,
    const std::vector<AttachmentDescription>& attachments):
    NonDispatchable(VK_OBJECT_TYPE_RENDER_PASS, std::move(device), std::move(allocator)),
    attachments(attachments),
    hash(0ull)
{}
} // namespace magma
