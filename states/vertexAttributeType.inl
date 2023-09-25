namespace magma
{
template<class PodType, int components, bool normalized, bool scaled>
constexpr Numeric VertexAttributeType<PodType, components, normalized, scaled>::numeric() noexcept
{
    if (std::is_integral<PodType>::value)
    {
        if (std::is_unsigned<PodType>::value)
        {
            if (scaled)
                return Numeric::UScaled;
            else
                return normalized ? Numeric::UNorm : Numeric::UInt;
        }
        else
        {
            if (scaled)
                return Numeric::SScaled;
            else
                return normalized ? Numeric::SNorm : Numeric::SInt;
        }
    }
    return Numeric::SFloat;
}

template<VkFormat Format>
constexpr VkFormat VertexAttributeFormat<Format>::format() noexcept
{
    constexpr magma::Format attribFormat(Format);
    static_assert(attribFormat.valid(), "vertex attribute type not specialized");
    static_assert(
        !attribFormat.depth() &&
        !attribFormat.stencil() &&
        !attribFormat.depthStencil(), "depth/stencil formats not allowed");
    static_assert(
        !attribFormat.bc() &&
        !attribFormat.etc2() &&
        !attribFormat.eac() &&
        !attribFormat.astcLdr() &&
        !attribFormat.astcHdr() &&
        !attribFormat.pvrtc(), "block compressed formats not allowed");
    static_assert(!attribFormat.srgb(), "sRGB formats not allowed");
    static_assert(!attribFormat.ycbcr(), "YCbCr formats not allowed");
    return Format;
}
} // namespace magma
