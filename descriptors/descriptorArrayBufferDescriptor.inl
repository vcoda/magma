class BufferDescriptor
{
    VkDescriptorBufferInfo& bufferDescriptor;
    const VkBufferUsageFlags requiredUsage;
    bool& updated;

public:
    explicit BufferDescriptor(VkDescriptorBufferInfo& bufferDescriptor, VkBufferUsageFlags requiredUsage, bool& updated) noexcept:
        bufferDescriptor(bufferDescriptor),
        requiredUsage(requiredUsage),
        updated(updated)
    {}

    void operator=(std::shared_ptr<const Buffer> buffer) noexcept
    {
        MAGMA_ASSERT(buffer);
        MAGMA_ASSERT(buffer->getUsage() & requiredUsage);
        if (bufferDescriptor.buffer != *buffer)
        {
            bufferDescriptor = buffer->getDescriptor();
            updated = true;
        }
    }
};
