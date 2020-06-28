namespace magma
{
inline bool BorderColor::isCustom() const noexcept
{
    return VK_BORDER_COLOR_MAX_ENUM == color;
}

template<typename Type>
inline TransparentBorderColor<Type>::TransparentBorderColor() noexcept:
    BorderColor(VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK)
{
}

template<typename Type, int x>
inline OpaqueBorderColor<Type, x>::OpaqueBorderColor() noexcept:
    BorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK)
{
}

template<>
class TransparentBorderColor<float> : public BorderColor
{
public:
    TransparentBorderColor() noexcept: BorderColor(VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK) {}
};

template<>
class TransparentBorderColor<int> : public BorderColor
{
public:
    TransparentBorderColor() noexcept: BorderColor(VK_BORDER_COLOR_INT_TRANSPARENT_BLACK) {}
};

template<>
class OpaqueBorderColor<float, 0> : public BorderColor
{
public:
    OpaqueBorderColor() noexcept: BorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK) {}
};

template<>
class OpaqueBorderColor<float, 1> : public BorderColor
{
public:
    OpaqueBorderColor() noexcept: BorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE) {}
};

template<>
class OpaqueBorderColor<int, 0> : public BorderColor
{
public:
    OpaqueBorderColor() noexcept: BorderColor(VK_BORDER_COLOR_INT_OPAQUE_BLACK) {}
};

template<>
class OpaqueBorderColor<int, 1> : public BorderColor
{
public:
    OpaqueBorderColor() noexcept: BorderColor(VK_BORDER_COLOR_INT_OPAQUE_WHITE) {}
};
} // namespace
