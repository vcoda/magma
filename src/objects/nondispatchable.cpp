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
#include "nondispatchable.h"
#include "device.h"
#include "privateDataSlot.h"

namespace magma
{
std::mutex NonDispatchableImpl::mtx;

void NonDispatchableImpl::setPrivateData(const IObject *parent, std::shared_ptr<Device> device, uint64_t data)
{
    if (!device)
        return;
#ifdef VK_EXT_private_data
    std::shared_ptr<PrivateDataSlot> privateDataSlot = device->getPrivateDataSlot();
    if (privateDataSlot)
    {
        privateDataSlot->setPrivateData(parent, data);
        return;
    }
#endif // VK_EXT_private_data
    std::lock_guard<std::mutex> lock(mtx);
    std::unordered_map<uint64_t, uint64_t>& privateData = device->getPrivateDataMap();
    privateData[parent->getObjectHandle()] = data;
}

uint64_t NonDispatchableImpl::getPrivateData(const IObject *parent, std::shared_ptr<Device> device) const noexcept
{
    if (!device)
        return 0ull;
#ifdef VK_EXT_private_data
    std::shared_ptr<PrivateDataSlot> privateDataSlot = device->getPrivateDataSlot();
    if (privateDataSlot)
        return privateDataSlot->getPrivateData(parent);
#endif // VK_EXT_private_data
    std::lock_guard<std::mutex> lock(mtx);
    std::unordered_map<uint64_t, uint64_t>& privateData = device->getPrivateDataMap();
    auto it = privateData.find(parent->getObjectHandle());
    if (it != privateData.end())
        return it->second;
    return 0ull;
}
} // namespace magma
