namespace magma
{
constexpr BorderColor::BorderColor(VkBorderColor color) noexcept:
    color(color),
    customColor{},
    format(VK_FORMAT_UNDEFINED)
{}

constexpr BorderColor::BorderColor(VkClearColorValue customColor, VkFormat format /* VK_FORMAT_UNDEFINED */) noexcept:
    color(VK_BORDER_COLOR_MAX_ENUM),
    customColor(customColor),
    format(format)
{}

constexpr bool BorderColor::custom() const noexcept
{
    return VK_BORDER_COLOR_MAX_ENUM == color;
}

constexpr std::size_t BorderColor::hash() const noexcept
{
    if (custom())
    {
        return core::hashArgs(
            customColor.float32[0],
            customColor.float32[1],
            customColor.float32[2],
            customColor.float32[3],
            format);
    }
    return core::hash(color);
}
} // namespace magma
