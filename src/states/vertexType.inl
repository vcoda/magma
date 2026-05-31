namespace magma
{
template<class T, std::size_t N, bool Normalized /* false */, bool Scaled /* false */>
constexpr NumericFormat AttributeType<T, N, Normalized, Scaled>::format() noexcept
{
    if constexpr (std::is_integral<T>::value)
    {
        if constexpr (std::is_unsigned<T>::value)
        {
            if constexpr (Scaled)
                return NumericFormat::UScaled;
            else if (Normalized)
                return NumericFormat::UNorm;
            else
                return NumericFormat::UInt;
        }
        else
        {
            if constexpr (Scaled)
                return NumericFormat::SScaled;
            else if constexpr (Normalized)
                return NumericFormat::SNorm;
            else
                return NumericFormat::SInt;
        }
    }
    return NumericFormat::SFloat;
}

template<VkFormat Format>
constexpr VkFormat AttributeFormat<Format>::format() noexcept
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
