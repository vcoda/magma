namespace magma
{
constexpr Scissor::Scissor(const int32_t x, const int32_t y, const VkExtent2D& extent) noexcept:
    VkRect2D{{x, y}, extent}
{}

constexpr Scissor::Scissor(const float x, const float y, const VkExtent2D& extent) noexcept:
    VkRect2D{
        VkOffset2D{
            static_cast<int32_t>(x),
            static_cast<int32_t>(y)
        }, extent
    }
{}

constexpr Scissor::Scissor(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) noexcept:
    VkRect2D{{x, y}, {width, height}}
{}

constexpr Scissor::Scissor(const float x, const float y, const float width, const float height) noexcept:
    VkRect2D{
        VkOffset2D{
            static_cast<int32_t>(x),
            static_cast<int32_t>(y)
        }, VkExtent2D{
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        }
    }
{}
} // namespace magma
