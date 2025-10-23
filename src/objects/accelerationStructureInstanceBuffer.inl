namespace magma
{
template<class Instance>
inline AccelerationStructureInstanceBuffer<Instance>::AccelerationStructureInstanceBuffer(
    std::shared_ptr<Device> device_, uint32_t instanceCount,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    AccelerationStructureInputBuffer(std::move(device_), sizeof(Instance) * instanceCount,
        optional, sharing, allocator),
    instanceCount(instanceCount),
    first(std::numeric_limits<uint32_t>::max()),
    last(std::numeric_limits<uint32_t>::min())
{
    srcBuffer = std::make_unique<SrcTransferBuffer>(device, size, std::move(allocator));
    instances = reinterpret_cast<Instance *>(srcBuffer->getMemory()->map());
    MAGMA_ASSERT(instances);
    if (instances)
    {   // Call constructor of each instance
        for (uint32_t i = 0; i < instanceCount; ++i)
            new (&instances[i]) Instance();
    }
}

template<class Instance>
inline Instance& AccelerationStructureInstanceBuffer<Instance>::getInstance(uint32_t index) noexcept
{
    MAGMA_ASSERT(index < instanceCount);
    first = std::min(first, index);
    last = std::max(last, index);
    return instances[index];
}

template<class Instance>
inline void AccelerationStructureInstanceBuffer<Instance>::updateModified(lent_ptr<CommandBuffer> cmdBuffer)
{
    if (last >= first)
    {
        const VkDeviceSize offset = sizeof(Instance) * first;
        const VkDeviceSize size = sizeof(Instance) * (last - first + 1);
        transferCopy(std::move(cmdBuffer), srcBuffer, offset, offset, size);
        first = std::numeric_limits<uint32_t>::max();
        last = std::numeric_limits<uint32_t>::min();
    }
}

template<class Instance>
inline void AccelerationStructureInstanceBuffer<Instance>::updateWhole(lent_ptr<CommandBuffer> cmdBuffer)
{
    transferCopy(std::move(cmdBuffer), srcBuffer, 0, 0, VK_WHOLE_SIZE);
}
} // namespace magma
