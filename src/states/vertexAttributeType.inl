namespace magma
{
template<class PodType, int N, bool Normalized, bool Scaled>
constexpr Numeric VertexAttributeType<PodType, N, Normalized, Scaled>::getNumeric() noexcept
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
constexpr VkFormat VertexAttributeFormat<Format>::getFormat() noexcept
{
    constexpr magma::Format format(Format);
    static_assert(format.valid(), "vertex attribute type not specialized");
    static_assert(
        !format.depth() &&
        !format.stencil() &&
        !format.depthStencil(), "depth/stencil formats not allowed");
    static_assert(
        !format.bc() &&
        !format.etc2() &&
        !format.eac() &&
        !format.astcLdr() &&
        !format.astcHdr() &&
        !format.pvrtc(), "block compressed formats not allowed");
    static_assert(!format.srgb(), "sRGB formats not allowed");
    static_assert(!format.ycbcr(), "YCbCr formats not allowed");
    static_assert(!format.packed(), "packed formats not allowed");
    static_assert(!format.multiPlanar(), "multi-planar formats not allowed");
    return Format;
}
} // namespace magma
