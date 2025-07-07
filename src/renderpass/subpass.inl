namespace magma
{
constexpr SubpassDescription::SubpassDescription(VkPipelineBindPoint pipelineBindPoint /* VK_PIPELINE_BIND_POINT_GRAPHICS */) noexcept:
    VkSubpassDescription{
        0, // flags
        pipelineBindPoint,
        0, // inputAttachmentCount
        nullptr, // pInputAttachments
        0, // colorAttachmentCount
        nullptr, // pColorAttachments
        nullptr, // pResolveAttachments
        nullptr, // pDepthStencilAttachment
        0, // preserveAttachmentCount
        nullptr // pPreserveAttachments
    }
{}
} // namespace magma
