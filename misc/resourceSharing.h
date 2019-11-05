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
#pragma once

namespace magma
{
    class PhysicalDevice;

    /* Buffer and image objects may be created with a concurrent sharing mode.
       Concurrent access is supported for multiple queue families. */

    class ResourceSharing
    {
    public:
        ResourceSharing() = default;
        ResourceSharing(const std::vector<uint32_t>& queueFamilyIndices);
        ResourceSharing(std::shared_ptr<const PhysicalDevice> device,
            const std::initializer_list<VkQueueFlagBits>& queueTypes);
        VkSharingMode getMode() const noexcept;
        uint32_t getQueueFamiliesCount() const noexcept;
        const std::vector<uint32_t>& getQueueFamilyIndices() const noexcept;

    private:
        uint32_t chooseFamilyIndex(VkQueueFlagBits queueType,
            const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const noexcept;

        std::vector<uint32_t> queueFamilyIndices;
    };
} // namespace magma
