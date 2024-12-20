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

namespace magma
{
    class PhysicalDevice;

    /* Buffer and image objects are created with a sharing mode
       controlling how they can be accessed from queues.
        * VK_SHARING_MODE_EXCLUSIVE specifies that access to any
          range or image subresource of the object will be
          exclusive to a single queue family at a time.
        * VK_SHARING_MODE_CONCURRENT specifies that concurrent
          access to any range or image subresource of the object
          from multiple queue families is supported. */

    class Sharing
    {
    public:
        Sharing() = default;
        explicit Sharing(const std::vector<uint32_t>& queueFamilyIndices) noexcept;
        explicit Sharing(const std::shared_ptr<PhysicalDevice>& device,
            const std::initializer_list<VkQueueFlagBits>& queueTypes);
        VkSharingMode getMode() const noexcept;
        uint32_t getQueueFamiliesCount() const noexcept;
        const std::vector<uint32_t>& getQueueFamilyIndices() const noexcept { return queueFamilyIndices; }

    private:
        uint32_t chooseFamilyIndex(VkQueueFlagBits queueType,
            const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const noexcept;

        std::vector<uint32_t> queueFamilyIndices;
    };
} // namespace magma
