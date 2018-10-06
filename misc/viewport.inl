namespace magma
{
MAGMA_INLINE Viewport::Viewport(float x, float y, float width, float height,
    float minDepth /* 0.f */, float maxDepth /* 1.f */) noexcept
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->minDepth = minDepth;
    this->maxDepth = maxDepth;
}

MAGMA_INLINE Viewport::Viewport(float x, float y, const VkExtent2D& extent,
    float minDepth /* 0.f */, float maxDepth /* 1.f */) noexcept
{
    this->x = x;
    this->y = y;
    width = static_cast<float>(extent.width);
    height = static_cast<float>(extent.height);
    this->minDepth = minDepth;
    this->maxDepth = maxDepth;
}

MAGMA_INLINE Viewport::Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
    float minDepth /* 0.f */, float maxDepth /* 1.f */) noexcept
{
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    this->width = static_cast<float>(width);
    this->height = static_cast<float>(height);
    this->minDepth = minDepth;
    this->maxDepth = maxDepth;
}
} // namespace magma
