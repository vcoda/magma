namespace magma
{
namespace descriptor
{
class DescriptorArray::BufferDescriptor
{
    VkDescriptorBufferInfo& descriptor;
    const VkBufferUsageFlags requiredUsage;
    bool& updated;

public:
    explicit BufferDescriptor(VkDescriptorBufferInfo& descriptor, VkBufferUsageFlags requiredUsage, bool& updated) noexcept:
        descriptor(descriptor),
        requiredUsage(requiredUsage),
        updated(updated)
    {}

    void operator=(std::shared_ptr<const Buffer> buffer) noexcept
    {
        MAGMA_ASSERT(buffer);
        MAGMA_ASSERT(buffer->getUsage() & requiredUsage);
        if (descriptor.buffer != *buffer)
        {
            descriptor = buffer->getDescriptor();
            updated = true;
        }
    }
};

class DescriptorArray::TexelBufferDescriptor
{
    VkBufferView& view;
    const VkBufferUsageFlags requiredUsage;
    bool& updated;

public:
    explicit TexelBufferDescriptor(VkBufferView& view, VkBufferUsageFlags requiredUsage, bool& updated) noexcept:
        view(view),
        requiredUsage(requiredUsage),
        updated(updated)
    {}

    void operator=(std::shared_ptr<const BufferView> bufferView) noexcept
    {
        MAGMA_ASSERT(bufferView);
        MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & requiredUsage);
        if (view != *bufferView)
        {
            view = *bufferView;
            updated = true;
        }
    }
};
} // namespace descriptor
} // namespace magma
