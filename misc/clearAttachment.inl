namespace magma
{
constexpr ClearAttachment::ClearAttachment():
    VkClearAttachment{}
{}

constexpr ColorClearAttachment::ColorClearAttachment(uint32_t attachment)
{
    aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    colorAttachment = attachment;
}

constexpr ColorClearAttachment::ColorClearAttachment(uint32_t attachment, float r, float g, float b, float a /* 1.f */):
    ColorClearAttachment(attachment)
{
    clearValue.color.float32[0] = r;
    clearValue.color.float32[1] = g;
    clearValue.color.float32[2] = b;
    clearValue.color.float32[3] = a;
}

constexpr ColorClearAttachment::ColorClearAttachment(uint32_t attachment, int32_t r, int32_t g, int32_t b, int32_t a):
    ColorClearAttachment(attachment)
{
    clearValue.color.int32[0] = r;
    clearValue.color.int32[1] = g;
    clearValue.color.int32[2] = b;
    clearValue.color.int32[3] = a;
}

constexpr ColorClearAttachment::ColorClearAttachment(uint32_t attachment, uint32_t r, uint32_t g, uint32_t b, uint32_t a):
    ColorClearAttachment(attachment)
{
    clearValue.color.uint32[0] = r;
    clearValue.color.uint32[1] = g;
    clearValue.color.uint32[2] = b;
    clearValue.color.uint32[3] = a;
}

constexpr ColorClearAttachment::ColorClearAttachment(uint32_t attachment, const float color[4]):
    ColorClearAttachment(attachment)
{
    clearValue.color.float32[0] = color[0];
    clearValue.color.float32[1] = color[1];
    clearValue.color.float32[2] = color[2];
    clearValue.color.float32[3] = color[3];
}

constexpr ColorClearAttachment::ColorClearAttachment(uint32_t attachment, const int32_t color[4]):
    ColorClearAttachment(attachment)
{
    clearValue.color.int32[0] = color[0];
    clearValue.color.int32[1] = color[1];
    clearValue.color.int32[2] = color[2];
    clearValue.color.int32[3] = color[3];
}

constexpr ColorClearAttachment::ColorClearAttachment(uint32_t attachment, const uint32_t color[4]):
    ColorClearAttachment(attachment)
{
    clearValue.color.uint32[0] = color[0];
    clearValue.color.uint32[1] = color[1];
    clearValue.color.uint32[2] = color[2];
    clearValue.color.uint32[3] = color[3];
}

constexpr ColorClearAttachment::ColorClearAttachment(uint32_t attachment, const ClearColor& color):
    ColorClearAttachment(attachment)
{
    clearValue = color;
}

constexpr DepthClearAttachment::DepthClearAttachment(float depth)
{
    aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    colorAttachment = 0;
    clearValue.depthStencil.depth = depth;
    clearValue.depthStencil.stencil = 0;
}

constexpr StencilClearAttachment::StencilClearAttachment(uint8_t stencil)
{
    aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
    colorAttachment = 0;
    clearValue.depthStencil.depth = 0.f;
    clearValue.depthStencil.stencil = stencil;
}

constexpr DepthStencilClearAttachment::DepthStencilClearAttachment(float depth, uint8_t stencil)
{
    aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    colorAttachment = 0;
    clearValue.depthStencil.depth = depth;
    clearValue.depthStencil.stencil = stencil;
}

constexpr DepthStencilClearAttachment::DepthStencilClearAttachment(const ClearDepthStencil& depthStencil)
{
    aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    colorAttachment = 0;
    clearValue = depthStencil;
}
} // namespace magma
