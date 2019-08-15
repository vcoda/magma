namespace magma
{
namespace aux
{
inline void ImmediateRender::setVertexShader(const VertexShaderStage& shader) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    vertexShader = shader;
}

inline void ImmediateRender::setFragmentShader(const FragmentShaderStage& shader) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    fragmentShader = shader;
}

inline void ImmediateRender::setRasterizationState(const RasterizationState& state) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    renderStates.rasterization = state;
}

inline void ImmediateRender::setMultisampleState(const MultisampleState& state) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    renderStates.multisample = state;
}

inline void ImmediateRender::setDepthStencilState(const DepthStencilState& state) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    renderStates.depthStencil = state;
}

inline void ImmediateRender::setColorBlendState(const ColorBlendState& state) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    renderStates.colorBlend = ManagedColorBlendState(state);
}

inline void ImmediateRender::setLineWidth(float width) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    lineWidth = width;
}

inline void ImmediateRender::setIdentity() noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    transform = Transform();
}

inline void ImmediateRender::setTransform(const float transform[16]) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    memcpy(this->transform.m, transform, sizeof(float) * 16);
}

inline uint32_t ImmediateRender::getVertexCount() const noexcept 
{ 
    return vertexCount; 
}

inline uint32_t ImmediateRender::getPrimitiveCount() const noexcept 
{ 
    return MAGMA_COUNT(primitives); 
}

inline void ImmediateRender::normal(float x, float y, float z) noexcept
{
    curr.nx = x;
    curr.ny = y;
    curr.nz = z;
}

inline void ImmediateRender::normal(const float n[3]) noexcept
{
    normal(n[0], n[1], n[2]);
}

inline void ImmediateRender::color(float r, float g, float b, float a /* 1 */) noexcept
{
    curr.r = r;
    curr.g = g;
    curr.b = b;
    curr.a = a;
}

inline void ImmediateRender::color(const float c[4]) noexcept
{
    color(c[0], c[1], c[2], c[3]);
}

inline void ImmediateRender::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a /* std::numeric_limits<uint8_t>::max() */) noexcept
{
    color(r/255.f, g/255.f, b/255.f, a/255.f);
}

inline void ImmediateRender::color(const uint8_t c[4]) noexcept
{
    color(c[0], c[1], c[2], c[3]);
}

inline void ImmediateRender::texcoord(float u, float v) noexcept
{
    curr.u = u;
    curr.v = v;
}

inline void ImmediateRender::texcoord(const float uv[2]) noexcept
{
    texcoord(uv[0], uv[1]);
}

inline void ImmediateRender::pointSize(float size) noexcept
{
    MAGMA_ASSERT(size >= 1.f);
    curr.psize = size;
}

inline void ImmediateRender::vertex(float x, float y, float z /* 0 */, float w /* 1 */) noexcept
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

inline void ImmediateRender::vertex(const float v[4]) noexcept
{
    vertex(v[0], v[1], v[2], v[3]);
}
} // namespace aux
} // namespace magma
