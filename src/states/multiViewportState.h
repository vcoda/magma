/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "viewportState.h"

namespace magma
{
    /* If VkPhysicalDeviceFeatures::multiViewport is supported,
       then multiple viewports and scissor rectangles can be defined. */

    class MultiViewportState : public ViewportState
    {
    public:
        explicit MultiViewportState(const std::vector<VkViewport>& viewports);
        explicit MultiViewportState(const std::vector<VkViewport>& viewports,
            const std::vector<VkRect2D>& scissors);
        MultiViewportState(const MultiViewportState&);
        MultiViewportState& operator=(const MultiViewportState&);
        hash_t hash() const noexcept override;
        bool operator==(const MultiViewportState&) const noexcept;

    private:
        std::vector<VkViewport> viewports;
        std::vector<VkRect2D> scissors;
    };
} // namespace magma
