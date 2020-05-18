namespace magma
{
constexpr ClearAttachment::ClearAttachment(const VkImageAspectFlags aspectMask, const uint32_t attachment) noexcept:
    VkClearAttachment{
        aspectMask,
        attachment,
        VkClearValue{}
    }
{}

constexpr ColorClearAttachment::ColorClearAttachment(const uint32_t attachment, const float r, const float g, const float b, const float a /* 1.f */) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_COLOR_BIT, attachment)
{
    clearValue.color.float32[0] = r;
    clearValue.color.float32[1] = g;
    clearValue.color.float32[2] = b;
    clearValue.color.float32[3] = a;
}

constexpr ColorClearAttachment::ColorClearAttachment(const uint32_t attachment, const int32_t r, const int32_t g, const int32_t b, const int32_t a) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_COLOR_BIT, attachment)
{
    clearValue.color.int32[0] = r;
    clearValue.color.int32[1] = g;
    clearValue.color.int32[2] = b;
    clearValue.color.int32[3] = a;
}

constexpr ColorClearAttachment::ColorClearAttachment(const uint32_t attachment, const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_COLOR_BIT, attachment)
{
    clearValue.color.uint32[0] = r;
    clearValue.color.uint32[1] = g;
    clearValue.color.uint32[2] = b;
    clearValue.color.uint32[3] = a;
}

constexpr ColorClearAttachment::ColorClearAttachment(const uint32_t attachment, const float color[4]) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_COLOR_BIT, attachment)
{
    clearValue.color.float32[0] = color[0];
    clearValue.color.float32[1] = color[1];
    clearValue.color.float32[2] = color[2];
    clearValue.color.float32[3] = color[3];
}

constexpr ColorClearAttachment::ColorClearAttachment(const uint32_t attachment, const int32_t color[4]) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_COLOR_BIT, attachment)
{
    clearValue.color.int32[0] = color[0];
    clearValue.color.int32[1] = color[1];
    clearValue.color.int32[2] = color[2];
    clearValue.color.int32[3] = color[3];
}

constexpr ColorClearAttachment::ColorClearAttachment(const uint32_t attachment, const uint32_t color[4]) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_COLOR_BIT, attachment)
{
    clearValue.color.uint32[0] = color[0];
    clearValue.color.uint32[1] = color[1];
    clearValue.color.uint32[2] = color[2];
    clearValue.color.uint32[3] = color[3];
}

constexpr ColorClearAttachment::ColorClearAttachment(const uint32_t attachment, const ClearColor& color) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_COLOR_BIT, attachment)
{
    clearValue = color;
}

constexpr DepthClearAttachment::DepthClearAttachment(const float depth) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_DEPTH_BIT, 0)
{
    clearValue.depthStencil.depth = depth;
    clearValue.depthStencil.stencil = 0;
}

constexpr StencilClearAttachment::StencilClearAttachment(const uint8_t stencil) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_STENCIL_BIT, 0)
{
    clearValue.depthStencil.depth = 0.f;
    clearValue.depthStencil.stencil = stencil;
}

constexpr DepthStencilClearAttachment::DepthStencilClearAttachment(const float depth, const uint8_t stencil) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0)
{
    clearValue.depthStencil.depth = depth;
    clearValue.depthStencil.stencil = stencil;
}

constexpr DepthStencilClearAttachment::DepthStencilClearAttachment(const ClearDepthStencil& depthStencil) noexcept:
    ClearAttachment(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0)
{
    clearValue = depthStencil;
}
} // namespace magma
