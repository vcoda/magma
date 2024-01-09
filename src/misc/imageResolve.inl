namespace magma
{
inline ImageResolve::ImageResolve(std::shared_ptr<const Image> srcImage, std::shared_ptr<const Image> dstImage) noexcept:
    VkImageResolve{
        srcImage->getSubresourceLayers(0),
        VkOffset3D{0, 0, 0},
        dstImage->getSubresourceLayers(0),
        VkOffset3D{0, 0, 0},
        srcImage->getExtent()
    }
{}

inline ImageResolve::ImageResolve(std::shared_ptr<const Image> srcImage, uint32_t srcMipLevel,
    std::shared_ptr<const Image> dstImage, uint32_t dstMipLevel) noexcept:
    VkImageResolve{
        srcImage->getSubresourceLayers(srcMipLevel),
        VkOffset3D{0, 0, 0},
        dstImage->getSubresourceLayers(dstMipLevel),
        VkOffset3D{0, 0, 0},
        srcImage->getExtent()
    }
{}
} // namespace magma
