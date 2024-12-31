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
#include "pch.h"
#pragma hdrstop
#include "deviceFaultInfo.h"

namespace magma
{
#ifdef VK_EXT_device_fault
DeviceFaultInfo::DeviceFaultInfo() noexcept:
    VkDeviceFaultInfoEXT{
        VK_STRUCTURE_TYPE_DEVICE_FAULT_INFO_EXT,
        nullptr,
        "",
        nullptr,
        nullptr,
        nullptr
    }
{}

DeviceFaultInfo::DeviceFaultInfo(const VkDeviceFaultCountsEXT& faultCounts) noexcept:
    VkDeviceFaultInfoEXT{
        VK_STRUCTURE_TYPE_DEVICE_FAULT_INFO_EXT,
        nullptr,
        "",
        MAGMA_NEW VkDeviceFaultAddressInfoEXT[faultCounts.addressInfoCount],
        MAGMA_NEW VkDeviceFaultVendorInfoEXT[faultCounts.vendorInfoCount],
        MAGMA_NEW uint8_t[static_cast<std::size_t>(faultCounts.vendorBinarySize)]
    },
    addressInfoCount(faultCounts.addressInfoCount),
    vendorInfoCount(faultCounts.vendorInfoCount),
    vendorBinarySize(faultCounts.vendorBinarySize)
{}

DeviceFaultInfo::DeviceFaultInfo(const DeviceFaultInfo& other) noexcept:
    VkDeviceFaultInfoEXT{
        other.sType,
        other.pNext,
        "",
        core::copyArray(other.pAddressInfos, other.addressInfoCount),
        core::copyArray(other.pVendorInfos, other.vendorInfoCount),
        core::copyBinaryData(other.pVendorBinaryData, static_cast<std::size_t>(other.vendorBinarySize))
    },
    addressInfoCount(other.addressInfoCount),
    vendorInfoCount(other.vendorInfoCount),
    vendorBinarySize(other.vendorBinarySize)
{
    core::copyString(description, VK_MAX_DESCRIPTION_SIZE, other.description);
}

DeviceFaultInfo::DeviceFaultInfo(DeviceFaultInfo&& other) noexcept:
    VkDeviceFaultInfoEXT{
        other.sType,
        other.pNext,
        "",
        other.pAddressInfos,
        other.pVendorInfos,
        other.pVendorBinaryData,
    },
    addressInfoCount(other.addressInfoCount),
    vendorInfoCount(other.vendorInfoCount),
    vendorBinarySize(other.vendorBinarySize)
{
    core::copyString(description, VK_MAX_DESCRIPTION_SIZE, other.description);
    other.pNext = nullptr;
    other.pAddressInfos = nullptr;
    other.pVendorInfos = nullptr;
    other.pVendorBinaryData = nullptr;
    other.addressInfoCount = 0;
    other.vendorInfoCount = 0;
    other.vendorBinarySize = 0;
}

DeviceFaultInfo& DeviceFaultInfo::operator=(const DeviceFaultInfo& other) noexcept
{
    if (this != &other)
    {
        pNext = other.pNext;
        core::copyString(description, VK_MAX_DESCRIPTION_SIZE, other.description);
        delete[] pAddressInfos;
        pAddressInfos = core::copyArray(other.pAddressInfos, other.addressInfoCount);
        delete[] pVendorInfos;
        pVendorInfos = core::copyArray(other.pVendorInfos, other.vendorInfoCount);
        delete[] (uint8_t *)pVendorBinaryData;
        pVendorBinaryData = core::copyBinaryData(other.pVendorBinaryData, static_cast<std::size_t>(other.vendorBinarySize));
        addressInfoCount = other.addressInfoCount;
        vendorInfoCount = other.vendorInfoCount;
        vendorBinarySize = other.vendorBinarySize;
    }
    return *this;
}

DeviceFaultInfo::~DeviceFaultInfo()
{
    delete[] pAddressInfos;
    delete[] pVendorInfos;
    delete[] (uint8_t *)pVendorBinaryData;
}
#endif // VK_EXT_device_fault
} // namespace magma
