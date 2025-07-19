namespace magma
{
constexpr SubpassDescription::SubpassDescription(const VkPipelineBindPoint pipelineBindPoint /* VK_PIPELINE_BIND_POINT_GRAPHICS */) noexcept:
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

inline SubpassDescription::SubpassDescription(SubpassDescription&& other) noexcept:
    VkSubpassDescription{
        flags,
        other.pipelineBindPoint,
        other.inputAttachmentCount,
        other.pInputAttachments,
        other.colorAttachmentCount,
        other.pColorAttachments,
        other.pResolveAttachments,
        other.pDepthStencilAttachment,
        other.preserveAttachmentCount,
        other.pPreserveAttachments
    }
{
    other.flags = 0;
    other.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    other.inputAttachmentCount = 0;
    other.pInputAttachments = nullptr;
    other.colorAttachmentCount = 0;
    other.pColorAttachments = nullptr;
    other.pResolveAttachments = nullptr;
    other.pDepthStencilAttachment = nullptr;
    other.preserveAttachmentCount = 0;
    other.pPreserveAttachments = nullptr;
}

inline SubpassDescription& SubpassDescription::operator=(SubpassDescription&& other) noexcept
{
    if (this != &other)
    {
        this->~SubpassDescription();
        flags = other.flags;
        pipelineBindPoint = other.pipelineBindPoint;
        inputAttachmentCount = other.inputAttachmentCount;
        pInputAttachments = other.pInputAttachments;
        colorAttachmentCount = other.colorAttachmentCount;
        pColorAttachments = other.pColorAttachments;
        pResolveAttachments = other.pResolveAttachments;
        pDepthStencilAttachment = other.pDepthStencilAttachment;
        preserveAttachmentCount = other.preserveAttachmentCount;
        pPreserveAttachments = other.pPreserveAttachments;
        other.flags = 0;
        other.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        other.inputAttachmentCount = 0;
        other.pInputAttachments = nullptr;
        other.colorAttachmentCount = 0;
        other.pColorAttachments = nullptr;
        other.pResolveAttachments = nullptr;
        other.pDepthStencilAttachment = nullptr;
        other.preserveAttachmentCount = 0;
        other.pPreserveAttachments = nullptr;
    }
    return *this;
}
} // namespace magma
