/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#pragma once
#include "renderPass.h"

namespace magma
{
    /* Multiview is a rendering technique originally designed for VR
       where it is more efficient to record a single set of commands
       to be executed with slightly different behavior for each "view". */

#ifdef VK_KHR_multiview
    class MultiviewRenderPass : public RenderPass
    {
    public:
        explicit MultiviewRenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            uint32_t viewMask,
            uint32_t correlationMask,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        explicit MultiviewRenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            const std::vector<uint32_t>& viewMasks,
            const std::vector<int32_t> viewOffsets,
            const std::vector<uint32_t>& correlationMasks,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        const std::vector<uint32_t>& getViewMasks() const noexcept { return viewMasks; }
        const std::vector<int32_t>& getViewOffsets() const noexcept { return viewOffsets; }
        const std::vector<uint32_t>& getCorrelationMasks() const noexcept { return correlationMasks; }

    private:
        const std::vector<uint32_t> viewMasks;
        const std::vector<int32_t> viewOffsets;
        const std::vector<uint32_t> correlationMasks;
    };
#endif // VK_KHR_multiview
} // namespace magma
