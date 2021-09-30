/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
#include "renderPassMultiview.h"

namespace magma
{
class RenderPassMultiviewCreateInfo : public CreateInfo
{
public:
    RenderPassMultiviewCreateInfo(const std::vector<uint32_t>& viewMasks,
        const std::vector<int32_t>& viewOffsets,
        const std::vector<uint32_t>& correlationMasks,
        const CreateInfo& renderPassInfoEx = CreateInfo()) noexcept:
        viewMasks(viewMasks),
        viewOffsets(viewOffsets),
        correlationMasks(correlationMasks)
    {
        multiviewInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO_KHR;
        multiviewInfo.pNext = renderPassInfoEx.getNode();
        multiviewInfo.subpassCount = MAGMA_COUNT(this->viewMasks);
        multiviewInfo.pViewMasks = this->viewMasks.data();
        multiviewInfo.dependencyCount = MAGMA_COUNT(this->viewOffsets);
        multiviewInfo.pViewOffsets = this->viewOffsets.data();
        multiviewInfo.correlationMaskCount = MAGMA_COUNT(this->correlationMasks);
        multiviewInfo.pCorrelationMasks = this->correlationMasks.data();
        hash = core::hashArgs(
            multiviewInfo.sType,
            multiviewInfo.subpassCount,
            multiviewInfo.dependencyCount,
            multiviewInfo.correlationMaskCount);
        for (uint32_t i = 0; i < multiviewInfo.subpassCount; ++i)
            core::hashCombine(hash, multiviewInfo.pViewMasks[i]);
        for (uint32_t i = 0; i < multiviewInfo.dependencyCount; ++i)
            core::hashCombine(hash, multiviewInfo.pViewOffsets[i]);
        for (uint32_t i = 0; i < multiviewInfo.correlationMaskCount; ++i)
            core::hashCombine(hash, multiviewInfo.pCorrelationMasks[i]);
        core::hashCombine(hash, renderPassInfoEx.getHash());
    }

    virtual const void *getNode() const noexcept
    {
        return &multiviewInfo;
    }

private:
    const std::vector<uint32_t> viewMasks;
    const std::vector<int32_t> viewOffsets;
    const std::vector<uint32_t> correlationMasks;
    VkRenderPassMultiviewCreateInfoKHR multiviewInfo;
};

MultiviewRenderPass::MultiviewRenderPass(std::shared_ptr<Device> device, const std::vector<AttachmentDescription>& attachments,
    uint32_t viewMask, uint32_t correlationMask,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const CreateInfo& renderPassInfoEx /* default */):
    RenderPass(std::move(device), attachments, std::move(allocator),
        RenderPassMultiviewCreateInfo({viewMask}, {/* viewOffsets */}, {correlationMask}, renderPassInfoEx)),
    viewMasks{viewMask},
    correlationMasks{correlationMask}
{}

MultiviewRenderPass::MultiviewRenderPass(std::shared_ptr<Device> device, const std::vector<AttachmentDescription>& attachments,
    const std::vector<uint32_t>& viewMasks, const std::vector<int32_t> viewOffsets, const std::vector<uint32_t>& correlationMasks,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const CreateInfo& renderPassInfoEx /* default */):
    RenderPass(std::move(device), attachments, std::move(allocator),
        RenderPassMultiviewCreateInfo(viewMasks, viewOffsets, correlationMasks, renderPassInfoEx)),
    viewMasks(viewMasks),
    viewOffsets(viewOffsets),
    correlationMasks(correlationMasks)
{}
} // namespace magma
