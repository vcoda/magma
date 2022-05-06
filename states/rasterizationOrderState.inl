constexpr RasterizationOrderState::RasterizationOrderState(const RasterizationState& state,
    const VkRasterizationOrderAMD rasterizationOrder) noexcept:
    RasterizationState(state),
    order{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD,
        state.pNext,
        rasterizationOrder
    }
{
    pNext = &order;
}

inline std::size_t RasterizationOrderState::hash() const noexcept
{
    std::size_t hash = RasterizationState::hash();
    core::hashCombine(hash, core::hashArgs(
        order.sType,
        order.rasterizationOrder));
    return hash;
}

constexpr bool RasterizationOrderState::operator==(const RasterizationOrderState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (order.rasterizationOrder == other.order.rasterizationOrder);
}
