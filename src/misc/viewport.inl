namespace magma
{
constexpr Viewport::Viewport(const float x, const float y, const float width, const float height,
    const float minDepth /* 0.f */, const float maxDepth /* 1.f */) noexcept:
    VkViewport{
        x, y,
        width, height,
        minDepth,
        maxDepth
    }
{}

constexpr Viewport::Viewport(const float x, const float y, const VkExtent2D& extent,
    const float minDepth /* 0.f */, const float maxDepth /* 1.f */) noexcept:
    VkViewport{
        x, y,
        static_cast<float>(extent.width),
        static_cast<float>(extent.height),
        minDepth,
        maxDepth
    }
{}

constexpr Viewport::Viewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height,
    const float minDepth /* 0.f */, const float maxDepth /* 1.f */) noexcept:
    VkViewport{
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(width),
        static_cast<float>(height),
        minDepth,
        maxDepth
    }
{}
} // namespace magma
