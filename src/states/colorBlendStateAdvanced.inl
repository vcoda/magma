namespace magma
{
inline AdvancedColorBlendState::AdvancedColorBlendState(const AdvancedColorBlendState& other) noexcept:
    ColorBlendState(other),
    colorBlendAdvancedStateInfo(other.colorBlendAdvancedStateInfo),
    attachments(other.attachments)
{
    pNext = &colorBlendAdvancedStateInfo;
    pAttachments = attachments.data();
}

inline AdvancedColorBlendState::AdvancedColorBlendState(AdvancedColorBlendState&& other) noexcept:
    ColorBlendState(std::move(other)),
    colorBlendAdvancedStateInfo(other.colorBlendAdvancedStateInfo),
    attachments(std::move(other.attachments))
{
    pNext = &colorBlendAdvancedStateInfo;
    pAttachments = attachments.data();
    other.pNext = nullptr;
    other.pAttachments = nullptr;
}
} // namespace magma
