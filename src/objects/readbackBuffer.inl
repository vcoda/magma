namespace magma
{
template<class Type>
inline ReadbackBuffer<Type>::ReadbackBuffer(std::shared_ptr<Device> device, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    uint32_t count /* 1 */,
    bool indirectUsage /* false */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Buffer(device, sizeof(Type) * count, 0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
            (indirectUsage ? VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT : 0),
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, allocator),
    stageMask(stageMask),
    count(count),
    staging(std::make_unique<DstTransferBuffer>(std::move(device), size, std::move(allocator), optional, sharing))
{}

template<class Type>
inline void ReadbackBuffer<Type>::readback(lent_ptr<CommandBuffer> cmdBuffer) const
{
    cmdBuffer->pipelineBarrier(stageMask, VK_PIPELINE_STAGE_TRANSFER_BIT,
        BufferMemoryBarrier(this, barrier::buffer::shaderWriteTransferRead));
    const VkBufferCopy region{0, 0, size};
    cmdBuffer->getLean().copyBuffer(this, staging.get(), region);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT,
        BufferMemoryBarrier(staging.get(), barrier::buffer::transferWriteHostRead));
}

template<class Type>
inline void ReadbackBuffer<Type>::setValue(const Type& value,
    lent_ptr<CommandBuffer> cmdBuffer) noexcept
{
    LeanCommandBuffer& leanCmd = cmdBuffer->getLean();
    if constexpr (std::is_same_v<Type, uint32_t>)
        leanCmd.fillBuffer(this, value, sizeof(uint32_t) * count);
    else
    {   // Fill repeatedly the Type array[count] storage with 4-byte words
        const uint32_t *src = (const uint32_t *)&value;
        for (uint32_t i = 0; i < count; ++i)
        {
            VkDeviceSize offset = i * sizeof(Type);
            constexpr uint32_t wordCount = sizeof(Type) / sizeof(uint32_t);
            for (uint32_t j = 0; j < wordCount; ++j)
            {
                uint32_t data = src[j];
                leanCmd.fillBuffer(this, data, sizeof(uint32_t), offset);
                offset += sizeof(uint32_t);
            }
        }
    }
    VkPipelineStageFlags dstStageMask = stageMask;
    VkAccessFlags dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
    if (getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT)
    {
        dstStageMask |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
        dstAccessMask |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
    }
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, dstStageMask,
        BufferMemoryBarrier(this, VK_ACCESS_TRANSFER_WRITE_BIT, dstAccessMask));
}

template<class Type>
bool ReadbackBuffer<Type>::getValue(Type *dst) const noexcept
{
    assert(dst);
    const void *src = staging->getMemory()->map();
    if (src)
    {
        memcpy(dst, src, sizeof(Type) * count);
        staging->getMemory()->unmap();
        return true;
    }
    return false;
}
} // namespace magma
