namespace magma
{
template<class PodType, int components, bool normalized, bool scaled>
constexpr Numeric AttributeType<PodType, components, normalized, scaled>::numeric() noexcept
{
    if (std::is_integral<PodType>::value)
    {
        if (std::is_unsigned<PodType>::value)
        {
            if (scaled)
                return Numeric::UScaled;
            else
                return Normalized ? Numeric::UNorm : Numeric::UInt;
        }
        else
        {
            if (scaled)
                return Numeric::SScaled;
            else
                return Normalized ? Numeric::SNorm : Numeric::SInt;
        }
    }
    return Numeric::SFloat;
}
} // namespace magma
