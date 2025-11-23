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
#include "image2D.h"

namespace magma
{
#ifdef VK_KHR_swapchain
    class Swapchain;

    /* The presentable image of a swapchain that is owned
       by the presentation engine. Presentable images can
       also be created by user and bound to swapchain memory.
       These images can be used anywhere swapchain images
       are used, and are useful in logical devices with
       multiple physical devices to create peer memory
       bindings of swapchain memory. All presentable images
       are initially in the VK_IMAGE_LAYOUT_UNDEFINED layout,
       thus before using presentable images, the application
       must transition them to a valid layout for the intended use. */

    class SwapchainImage : public Image2D
    {
    public:
    #ifdef VK_VERSION_1_1
        explicit SwapchainImage(lent_ptr<const Swapchain> swapchain);
    #endif
        ~SwapchainImage();
        int32_t getChainIndex() const noexcept { return chainIndex; }
        bool chained() const noexcept { return chainIndex >= 0; }
        void bindMemory(std::shared_ptr<IDeviceMemory> memory,
            VkDeviceSize offset = 0) override;
    #ifdef VK_KHR_device_group
        void bindMemoryDeviceGroup(std::shared_ptr<IDeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) override;
    #endif // VK_KHR_device_group

    private:
        MAGMA_MAKE_SHARED(SwapchainImage)
        SwapchainImage(VkImage handle,
            std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t arrayLayers,
            VkImageUsageFlags usage,
            uint32_t chainIndex);
        void setChainIndex(uint32_t chainIndex) noexcept;

        const bool implementationControlled;
        int32_t chainIndex;
        friend Swapchain;
    };
#endif // VK_KHR_swapchain
} // namespace magma
