namespace magma
{
constexpr Scissor::Scissor(int32_t x, int32_t y, const VkExtent2D& extent):
    VkRect2D{}
{
    offset.x = x;
    offset.y = y;
    this->extent = extent;
}

constexpr Scissor::Scissor(float x, float y, const VkExtent2D& extent):
    VkRect2D{}
{
    offset.x = static_cast<int32_t>(x);
    offset.y = static_cast<int32_t>(y);
    this->extent = extent;
}

constexpr Scissor::Scissor(int32_t x, int32_t y, uint32_t width, uint32_t height):
    VkRect2D{}
{
    offset.x = x;
    offset.y = y;
    extent.width = width;
    extent.height = height;
}

constexpr Scissor::Scissor(float x, float y, float width, float height):
    VkRect2D{}
{
    offset.x = static_cast<int32_t>(x);
    offset.y = static_cast<int32_t>(y);
    extent.width = static_cast<uint32_t>(width);
    extent.height = static_cast<uint32_t>(height);
}
} // namespace magma
