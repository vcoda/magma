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
#include "pch.h"
#pragma hdrstop
#include "specialization.h"

namespace magma
{
Specialization::Specialization(const Specialization& other)
{
    mapEntryCount = other.mapEntryCount;
    pMapEntries = internal::copyArray(other.pMapEntries, mapEntryCount);
    dataSize = other.dataSize;
    try {
        pData = internal::copyArray<char>(other.pData, dataSize);
    } catch (const std::bad_alloc& exc)
    {
        delete[] pMapEntries;
        throw exc;
    }
}

Specialization& Specialization::operator=(const Specialization& other)
{
    if (this != &other)
    {
        mapEntryCount = other.mapEntryCount;
        pMapEntries = internal::copyArray(other.pMapEntries, mapEntryCount);
        dataSize = other.dataSize;
        try {
            pData = internal::copyArray<char>(other.pData, dataSize);
        } catch (const std::bad_alloc& exc)
        {
            delete[] pMapEntries;
            throw exc;
        }
    }
    return *this;
}

Specialization::~Specialization()
{
    delete[] pMapEntries;
    delete[] reinterpret_cast<const char *>(pData);
}

size_t Specialization::hash() const noexcept
{
    size_t hash = 0;
    for (uint32_t i = 0; i < mapEntryCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pMapEntries[i].constantID,
            pMapEntries[i].offset,
            pMapEntries[i].size));
    }
    internal::hashCombine(hash, internal::hashArray(
        reinterpret_cast<const uint8_t *>(pData), dataSize));
    return hash;
}
} // namespace magma
