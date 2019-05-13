namespace magma
{
namespace aux
{
MAGMA_INLINE void ImmediateRender::setVertexShader(const VertexShaderStage& shader) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    vertexShader = shader;
}

MAGMA_INLINE void ImmediateRender::setFragmentShader(const FragmentShaderStage& shader) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    fragmentShader = shader;
}

MAGMA_INLINE void ImmediateRender::setRasterizationState(const RasterizationState& state) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    renderStates.rasterization = state;
}

MAGMA_INLINE void ImmediateRender::setMultisampleState(const MultisampleState& state) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    renderStates.multisample = state;
}

MAGMA_INLINE void ImmediateRender::setDepthStencilState(const DepthStencilState& state) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    renderStates.depthStencil = state;
}

MAGMA_INLINE void ImmediateRender::setColorBlendState(const ColorBlendState& state) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    renderStates.colorBlend = ManagedColorBlendState(state);
}

MAGMA_INLINE void ImmediateRender::setLineWidth(float width) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    lineWidth = width;
}

MAGMA_INLINE void ImmediateRender::setIdentity() noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    transform = Transform();
}

MAGMA_INLINE void ImmediateRender::setTransform(const float transform[16]) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    memcpy(this->transform.m, transform, sizeof(float) * 16);
}

MAGMA_INLINE void ImmediateRender::normal(
    float x, float y, float z) noexcept
{
    curr.nx = x;
    curr.ny = y;
    curr.nz = z;
}

MAGMA_INLINE void ImmediateRender::normal(
    const float n[3]) noexcept
{
    normal(n[0], n[1], n[2]);
}

MAGMA_INLINE void ImmediateRender::color(
    float r, float g, float b, float a /* 1 */) noexcept
{
    curr.r = r;
    curr.g = g;
    curr.b = b;
    curr.a = a;
}

MAGMA_INLINE void ImmediateRender::color(
    const float c[4]) noexcept
{
    color(c[0], c[1], c[2], c[3]);
}

MAGMA_INLINE void ImmediateRender::color(
    uint8_t r, uint8_t g, uint8_t b,
    uint8_t a /* UCHAR_MAX */) noexcept
{
    color(r/255.f, g/255.f, b/255.f, a/255.f);
}

MAGMA_INLINE void ImmediateRender::color(
    const uint8_t c[4]) noexcept
{
    color(c[0], c[1], c[2], c[3]);
}

MAGMA_INLINE void ImmediateRender::texCoord(
    float u, float v) noexcept
{
    curr.u = u;
    curr.v = v;
}

MAGMA_INLINE void ImmediateRender::texCoord(
    const float uv[2]) noexcept
{
    texCoord(uv[0], uv[1]);
}

MAGMA_INLINE void ImmediateRender::pointSize(
    float size) noexcept
{
    MAGMA_ASSERT(size >= 1.f);
    curr.psize = size;
}

MAGMA_INLINE void ImmediateRender::vertex(
    float x, float y, float z /* 0 */, float w /* 1 */) noexcept
{
    MAGMA_ASSERT(insidePrimitive);
    MAGMA_ASSERT(vertexCount < maxVertexCount);
    if (insidePrimitive && vertexCount < maxVertexCount)
    {   // Position
        vert->x = x;
        vert->y = y;
        vert->z = z;
        vert->w = w;
        // Normal
        vert->nx = curr.nx;
        vert->ny = curr.ny;
        vert->nz = curr.nz;
        // Color
        vert->r = curr.r;
        vert->g = curr.g;
        vert->b = curr.b;
        vert->a = curr.a;
        // Texture coords
        vert->u = curr.u;
        vert->v = curr.v;
        // Point size
        vert->psize = curr.psize;
        // Go to the next vertex
        ++vert;
        ++primitives.back().vertexCount;
        ++vertexCount;
    }
}

MAGMA_INLINE void ImmediateRender::vertex(
    const float v[4]) noexcept
{
    vertex(v[0], v[1], v[2], v[3]);
}

template<typename Normal>
MAGMA_INLINE void ImmediateRender::normal(
    const Normal& n) noexcept
{
    normal(n.x, n.y, n.z);
}

template<typename Color>
MAGMA_INLINE void ImmediateRender::color4(
    const Color& c) noexcept
{
    color(c.r, c.g, c.b, c.a);
}

template<typename TexCoord>
MAGMA_INLINE void ImmediateRender::texCoord2(
    const TexCoord& uv) noexcept
{
    texCoord(uv.x, uv.y);
}

template<typename Vertex2>
MAGMA_INLINE void ImmediateRender::vertex2(
    const Vertex2& v) noexcept
{
    vertex(v.x, v.y);
}

template<typename Vertex3>
MAGMA_INLINE void ImmediateRender::vertex3(
    const Vertex3& v) noexcept
{
    vertex(v.x, v.y, v.z);
}

template<typename Vertex4>
MAGMA_INLINE void ImmediateRender::vertex4(
    const Vertex4& v) noexcept
{
    vertex(v.x, v.y, v.z, v.w);
}
} // namespace aux
} // namespace magma
