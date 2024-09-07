namespace magma::packed
{
inline X11y11z10Ufloat::X11y11z10Ufloat(float x, float y, float z) noexcept
{
    const Float32 ivec[3] = {x, y, z};
    uint32_t res[3];
    // X & Y components (5-bit exponent, 6-bit mantissa)
    for (uint32_t j = 0; j < 2; ++j)
    {
        const uint32_t sign = ivec[j].i & 0x80000000;
        uint32_t i = ivec[j].i & 0x7FFFFFFF;
        if ((i & 0x7F800000) == 0x7F800000)
        {   // Inf or NaN
            res[j] = 0x7C0U;
            if ((i & 0x7FFFFF) != 0)
                res[j] = 0x7FFU;
            else if (sign)
                res[j] = 0; // -Inf is clamped to 0 because of unsigned type
        }
        else if (sign || i < 0x35800000)
            res[j] = 0; // Unsigned type, clamp to zero
        else if (i > 0x477E0000U)
            res[j] = 0x7BFU; // The number is too large to be represented as float11, set to max
        else
        {
            if (i < 0x38800000U)
            {   // The number is too small to be represented as a normalized float11.
                // Convert it to a denormalized value.
                const uint32_t shift = 113U - (i >> 23U);
                i = (0x800000U | (i & 0x7FFFFFU)) >> shift;
            }
            else
            {   // Rebias the exponent to represent the value as a normalized float11
                i += 0xC8000000U;
            }
            res[j] = ((i + 0xFFFFU + ((i >> 17U) & 1U)) >> 17U) & 0x7FFU;
        }
    }
    // Z component (5-bit exponent, 5-bit mantissa)
    const uint32_t sign = ivec[2].i & 0x80000000;
    uint32_t i = ivec[2].i & 0x7FFFFFFF;
    if ((i & 0x7F800000) == 0x7F800000)
    {   // Inf or NaN
        res[2] = 0x3E0U;
        if (i & 0x7FFFFF)
            res[2] = 0x3FFU;
        else if (sign || i < 0x36000000)
            res[2] = 0; // -Inf is clamped to 0 because of unsigned type
    }
    else if (sign)
        res[2] = 0; // Unsigned type, clamp to zero
    else if (i > 0x477C0000U)
        res[2] = 0x3DFU; // The number is too large to be represented as float10, set to max
    else
    {
        if (i < 0x38800000U)
        {   // The number is too small to be represented as a normalized float10.
            // Convert it to a denormalized value.
            const uint32_t shift = 113U - (i >> 23U);
            i = (0x800000U | (i & 0x7FFFFFU)) >> shift;
        }
        else
        {   // Rebias the exponent to represent the value as a normalized float10
            i += 0xC8000000U;
        }
        res[2] = ((i + 0x1FFFFU + ((i >> 18U) & 1U)) >> 18U) & 0x3FFU;
    }
    // Pack result into memory
    v = ((res[2] & 0x3FF) << 22) | ((res[1] & 0x7FF) << 11) | (res[0] & 0x7FF);
}
} // namespace magma::packed
