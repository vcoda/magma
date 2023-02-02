class BufferDescriptor
{
    const VkBufferUsageFlags requiredUsage;
    VkDescriptorBufferInfo& bufferDescriptor;
    bool& updated;

public:
    explicit BufferDescriptor(VkBufferUsageFlags requiredUsage, VkDescriptorBufferInfo& bufferDescriptor, bool& updated) noexcept:
        requiredUsage(requiredUsage),
        bufferDescriptor(bufferDescriptor),
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
