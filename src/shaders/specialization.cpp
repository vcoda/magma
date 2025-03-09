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
#include "specialization.h"

namespace magma
{
Specialization::Specialization(const Specialization& other) noexcept:
    VkSpecializationInfo{
        other.mapEntryCount,
        core::copyArray(other.pMapEntries, other.mapEntryCount),
        other.dataSize,
        core::copyBinaryData(other.pData, other.dataSize)
    }
{}

Specialization::Specialization(Specialization&& other) noexcept:
    VkSpecializationInfo{
        other.mapEntryCount,
        other.pMapEntries,
        other.dataSize,
        other.pData
    }
{
    other.mapEntryCount = 0;
    pMapEntries = nullptr;
    dataSize = 0;
    pData = nullptr;
}

Specialization& Specialization::operator=(const Specialization& other) noexcept
{
    if (this != &other)
    {
        this->~Specialization();
        mapEntryCount = other.mapEntryCount;
        pMapEntries = core::copyArray(other.pMapEntries, other.mapEntryCount);
        dataSize = other.dataSize;
        pData = core::copyBinaryData(other.pData, other.dataSize);
    }
    return *this;
}

Specialization& Specialization::operator=(Specialization&& other) noexcept
{
    if (this != &other)
    {
        this->~Specialization();
        mapEntryCount = other.mapEntryCount;
        pMapEntries = other.pMapEntries;
        dataSize = other.dataSize;
        pData = other.pData;
        other.mapEntryCount = 0;
        other.pMapEntries = nullptr;
        other.dataSize = 0;
        other.pData = nullptr;
    }
    return *this;
}

Specialization::~Specialization()
{
    delete[] pMapEntries;
    delete[] reinterpret_cast<const uint8_t *>(pData);
}

hash_t Specialization::getHash() const noexcept
{
    hash_t hash = 0;
    for (uint32_t i = 0; i < mapEntryCount; ++i)
    {
        const VkSpecializationMapEntry& entry = pMapEntries[i];
        hash = core::hashCombine(hash, core::hashArgs(
            entry.constantID,
            entry.offset,
            entry.size));
    }
    const uint8_t *byteData = reinterpret_cast<const uint8_t *>(pData);
    return core::hashCombine(hash, core::hashArray(
        byteData, dataSize));
}

std::ostream& operator<<(std::ostream& out, const Specialization& specialization)
{
    out << "\tVkSpecializationInfo [" << std::endl
        << "\t\tmapEntryCount: " << specialization.mapEntryCount << std::endl
        << "\t\tpMapEntries: ";
    if (!specialization.pMapEntries)
        out << "NULL" << std::endl;
    else
    {
        out << std::endl;
        for (uint32_t i = 0; i < specialization.mapEntryCount; ++i)
        {
            const VkSpecializationMapEntry& entry = specialization.pMapEntries[i];
            out << "\t\t[" << std::endl
                << "\t\t\tconstantID: " << entry.constantID << std::endl
                << "\t\t\toffset: " << entry.offset << std::endl
                << "\t\t\tsize: " << entry.size << std::endl
                << "\t\t]" << std::endl;
        }
    }
    out << "\t\tdataSize: " << specialization.dataSize << std::endl
        << "\t\tpData: ";
    if (!specialization.pData)
        out << "NULL" << std::endl;
    else
        out << "0x" << specialization.pData << std::endl;
    out << "\t]";
    return out;
}
} // namespace magma
