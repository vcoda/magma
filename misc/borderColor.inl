namespace magma
{
constexpr BorderColor::BorderColor(VkBorderColor color) noexcept:
    color(color),
    customColor{},
    format(VK_FORMAT_UNDEFINED)
{}

#ifdef VK_EXT_custom_border_color
constexpr BorderColor::BorderColor(VkClearColorValue customColor, VkFormat format /* VK_FORMAT_UNDEFINED */) noexcept:
    color(VK_BORDER_COLOR_FLOAT_CUSTOM_EXT),
    customColor(customColor),
    format(format)
{}

constexpr bool BorderColor::custom() const noexcept
{
    return VK_BORDER_COLOR_FLOAT_CUSTOM_EXT == color;
}
#endif // VK_EXT_custom_border_color

constexpr std::size_t BorderColor::hash() const noexcept
{
#ifdef VK_EXT_custom_border_color
    if (custom())
    {
        return core::hashArgs(
            customColor.float32[0],
            customColor.float32[1],
            customColor.float32[2],
            customColor.float32[3],
            format);
    }
#endif // VK_EXT_custom_border_color
    return core::hash(color);
}
} // namespace magma
