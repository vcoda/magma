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
#include "pch.h"
#pragma hdrstop
#include "deviceFaultInfo.h"
#include "../core/copy.h"

namespace magma
{
#ifdef VK_EXT_device_fault
DeviceFaultInfo::DeviceFaultInfo() noexcept
{
    sType = VK_STRUCTURE_TYPE_DEVICE_FAULT_INFO_EXT;
    pNext = nullptr;
    description[0] = '\0';
    pAddressInfos = nullptr;
    pVendorInfos = nullptr;
    pVendorBinaryData = nullptr;
}

DeviceFaultInfo::DeviceFaultInfo(const VkDeviceFaultCountsEXT& faultCounts):
    addressInfoCount(faultCounts.addressInfoCount),
    vendorInfoCount(faultCounts.vendorInfoCount),
    vendorBinarySize(faultCounts.vendorBinarySize)
{
    sType = VK_STRUCTURE_TYPE_DEVICE_FAULT_INFO_EXT;
    pNext = nullptr;
    description[0] = '\0';
    pAddressInfos = new VkDeviceFaultAddressInfoEXT[addressInfoCount];
    pVendorInfos = new VkDeviceFaultVendorInfoEXT[vendorInfoCount];
    pVendorBinaryData = new uint8_t[static_cast<std::size_t>(vendorBinarySize)];
}

DeviceFaultInfo::DeviceFaultInfo(const DeviceFaultInfo& other):
    addressInfoCount(other.addressInfoCount),
    vendorInfoCount(other.vendorInfoCount),
    vendorBinarySize(other.vendorBinarySize)
{
    sType = VK_STRUCTURE_TYPE_DEVICE_FAULT_INFO_EXT;
    pNext = other.pNext;
    strcpy_s(description, VK_MAX_DESCRIPTION_SIZE, other.description);
    pAddressInfos = core::copyArray(other.pAddressInfos, other.addressInfoCount);
    pVendorInfos = core::copyArray(other.pVendorInfos, other.vendorInfoCount);
    pVendorBinaryData = core::copyBinaryData(other.pVendorBinaryData, other.vendorBinarySize);
}

DeviceFaultInfo& DeviceFaultInfo::operator=(const DeviceFaultInfo& other)
{
    if (this != &other)
    {
        pNext = other.pNext;
        strcpy_s(description, VK_MAX_DESCRIPTION_SIZE, other.description);
        delete[] pAddressInfos;
        pAddressInfos = core::copyArray(other.pAddressInfos, other.addressInfoCount);
        addressInfoCount = other.addressInfoCount;
        delete[] pVendorInfos;
        pVendorInfos = core::copyArray(other.pVendorInfos, other.vendorInfoCount);
        vendorInfoCount = other.vendorInfoCount;
        delete[] pVendorBinaryData;
        pVendorBinaryData = core::copyBinaryData(other.pVendorBinaryData, other.vendorBinarySize);
        vendorBinarySize = other.vendorBinarySize;
    }
    return *this;
}

DeviceFaultInfo::~DeviceFaultInfo()
{
    delete[] pAddressInfos;
    delete[] pVendorInfos;
    delete[] pVendorBinaryData;
}
#endif // VK_EXT_device_fault
} // namespace magma
