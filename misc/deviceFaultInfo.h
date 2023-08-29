/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    /* Device loss can be triggered by a variety of issues,
       including invalid API usage, implementation errors,
       or hardware failures.

       Method Device::getFaultInfo() may be called subsequent
       to a VK_ERROR_DEVICE_LOST error code having been returned
       by the implementation. This method allows developers
       to query for additional information on GPU faults which
       may have caused device loss, and to generate binary
       crash dumps, which may be loaded into external tools
       for further diagnosis. */

#ifdef VK_EXT_device_fault
    class DeviceFaultInfo final : public VkDeviceFaultInfoEXT
    {
    public:
        DeviceFaultInfo() noexcept;
        explicit DeviceFaultInfo(const VkDeviceFaultCountsEXT& faultCounts);
        DeviceFaultInfo(const DeviceFaultInfo&) noexcept;
        DeviceFaultInfo(DeviceFaultInfo&&) noexcept;
        DeviceFaultInfo& operator=(const DeviceFaultInfo&) noexcept;
        ~DeviceFaultInfo();

    private:
        uint32_t addressInfoCount;
        uint32_t vendorInfoCount;
        VkDeviceSize vendorBinarySize;
    };
#endif // VK_EXT_device_fault
} // namespace magma
