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
    VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp,
    VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp,
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
    this->loadOp = loadOp;
    this->storeOp = storeOp;
    this->stencilLoadOp = stencilLoadOp;
    this->stencilStoreOp = stencilStoreOp;
    this->initialLayout = initialLayout;
    this->finalLayout = finalLayout;
}

constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount, 
    const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout) noexcept:
    AttachmentDescription(format, sampleCount, colorDepthOp.loadOp, colorDepthOp.storeOp,
        stencilOp.loadOp, stencilOp.storeOp, initialLayout, finalLayout)
{}

constexpr AttachmentDescription::AttachmentDescription(const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout) noexcept:
    AttachmentDescription(VK_FORMAT_UNDEFINED, VK_SAMPLE_COUNT_1_BIT, colorDepthOp.loadOp, colorDepthOp.storeOp,
        stencilOp.loadOp, stencilOp.storeOp, initialLayout, finalLayout)
{}

constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount, 
    const AttachmentDescription& prefab) noexcept:
    AttachmentDescription(format, sampleCount, prefab.loadOp, prefab.storeOp,
        prefab.stencilLoadOp, prefab.stencilStoreOp, initialLayout, finalLayout)
{}
} // namespace magma
