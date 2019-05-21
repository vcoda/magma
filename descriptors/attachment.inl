/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

namespace magma
{
constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount,
    const LoadStoreOp& op, const LoadStoreOp& stencilOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout) noexcept
{
    flags = 0;
    this->format = format;
    switch (sampleCount)
    {
    case 1: samples = VK_SAMPLE_COUNT_1_BIT; break;
    case 2: samples = VK_SAMPLE_COUNT_2_BIT; break;
    case 4: samples = VK_SAMPLE_COUNT_4_BIT; break;
    case 8: samples = VK_SAMPLE_COUNT_8_BIT; break;
    case 16: samples = VK_SAMPLE_COUNT_16_BIT; break;
    case 32: samples = VK_SAMPLE_COUNT_32_BIT; break;
    case 64: samples = VK_SAMPLE_COUNT_64_BIT; break;
    default: samples = VK_SAMPLE_COUNT_1_BIT;
    }
    loadOp = op.loadOp;
    storeOp = op.storeOp;
    stencilLoadOp = stencilOp.loadOp;
    stencilStoreOp = stencilOp.storeOp;
    this->initialLayout = initialLayout;
    this->finalLayout = finalLayout;
}

constexpr AttachmentDescription::AttachmentDescription(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp,
    VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout) noexcept
{
    flags = 0;
    format = VK_FORMAT_UNDEFINED;
    this->samples = VK_SAMPLE_COUNT_1_BIT;
    this->loadOp = loadOp;
    this->storeOp = storeOp;
    this->stencilLoadOp = stencilLoadOp;
    this->stencilStoreOp = stencilStoreOp;
    this->initialLayout = initialLayout;
    this->finalLayout = finalLayout;
}

constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount,
    const AttachmentDescription& predefined) noexcept
{
    *this = predefined;
    this->format = format;
    switch (sampleCount)
    {
    case 1: samples = VK_SAMPLE_COUNT_1_BIT; break;
    case 2: samples = VK_SAMPLE_COUNT_2_BIT; break;
    case 4: samples = VK_SAMPLE_COUNT_4_BIT; break;
    case 8: samples = VK_SAMPLE_COUNT_8_BIT; break;
    case 16: samples = VK_SAMPLE_COUNT_16_BIT; break;
    case 32: samples = VK_SAMPLE_COUNT_32_BIT; break;
    case 64: samples = VK_SAMPLE_COUNT_64_BIT; break;
    default: samples = VK_SAMPLE_COUNT_1_BIT;   
    }
}
} // namespace magma
