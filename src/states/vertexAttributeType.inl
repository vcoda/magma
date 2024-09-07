namespace magma
{
template<class PodType, int N, bool Normalized, bool Scaled>
constexpr Numeric VertexAttributeType<PodType, N, Normalized, Scaled>::numeric() noexcept
{
    if constexpr (std::is_integral<PodType>::value)
    {
        if constexpr (std::is_unsigned<PodType>::value)
        {
            if constexpr (Scaled)
                return Numeric::UScaled;
            else if (Normalized)
                return Numeric::UNorm;
            else
                return Numeric::UInt;
        }
        else
        {
            if constexpr (Scaled)
                return Numeric::SScaled;
            else if constexpr (Normalized)
                return Numeric::SNorm;
            else
                return Numeric::SInt;
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
    static_assert(!attribFormat.packed(), "packed formats not allowed");
    static_assert(!attribFormat.multiPlanar(), "multi-planar formats not allowed");
    return Format;
}
} // namespace magma
