namespace magma
{
MAGMA_INLINE ClearColor::ClearColor(float r, float g, float b, float a /* 1.f */) noexcept
{
    value.color.float32[0] = r;
    value.color.float32[1] = g;
    value.color.float32[2] = b;
    value.color.float32[3] = a;
}

MAGMA_INLINE ClearColor::ClearColor(int32_t r, int32_t g, int32_t b, int32_t a) noexcept
{
    value.color.int32[0] = r;
    value.color.int32[1] = g;
    value.color.int32[2] = b;
    value.color.int32[3] = a;
}

MAGMA_INLINE ClearColor::ClearColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept
{
    value.color.uint32[0] = r;
    value.color.uint32[1] = g;
    value.color.uint32[2] = b;
    value.color.uint32[3] = a;
}

MAGMA_INLINE ClearColor::ClearColor(const float color[4]) noexcept
{
    for (int i = 0; i < 4; ++i)
        value.color.float32[i] = color[i];
}

MAGMA_INLINE ClearColor::ClearColor(const int32_t color[4]) noexcept
{
    for (int i = 0; i < 4; ++i)
        value.color.int32[i] = color[i];
}

MAGMA_INLINE ClearColor::ClearColor(const uint32_t color[4]) noexcept
{
    for (int i = 0; i < 4; ++i)
        value.color.uint32[i] = color[i];
}

MAGMA_INLINE ClearColor::ClearColor(uint32_t hexValue) noexcept
{
    value.color.float32[0] = ((hexValue >> 16) & 0xFF) / static_cast<float>(0xFF);
    value.color.float32[1] = ((hexValue >> 8) & 0xFF) / static_cast<float>(0xFF);
    value.color.float32[2] = (hexValue & 0xFF) / static_cast<float>(0xFF);
    value.color.float32[3] = 1.f;
}

MAGMA_INLINE ClearDepthStencil::ClearDepthStencil(float depth, uint8_t stencil /* 0 */) noexcept
{
    value.depthStencil.depth = depth;
    value.depthStencil.stencil = stencil;
}
} // namespace magma
