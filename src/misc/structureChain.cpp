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
#include "pch.h"
#pragma hdrstop
#include "structureChain.h"

namespace magma
{
void StructureChain::insertNode(const ChainNode& node)
{
    emplace_back(node);
    VkBaseOutStructure *lastNode = nullptr;
    if (size() > 1)
    {   // Get only after possible reallocation
        lastNode = std::next(rbegin())->getNode();
        MAGMA_ASSERT(!lastNode->pNext);
    }
    VkBaseOutStructure *newNode = back().getNode();
    if (lastNode)
        lastNode->pNext = newNode;
    newNode->pNext = nullptr;
}

hash_t StructureChain::getHash() const noexcept
{
    hash_t hash = 0ull;
    std::for_each(begin(), end(),
        [&hash](auto& it)
        {
            hash = core::hashCombine(hash, it.getHash());
        });
    return hash;
}

VkBaseOutStructure *StructureChain::lookupNode(VkStructureType sType) noexcept
{
    auto it = std::find_if(begin(), end(),
        [sType](auto& it)
        {
           return (it.getNode()->sType == sType);
        });
    if (it != end())
        return it->getNode();
    return nullptr;
}

const VkBaseInStructure *StructureChain::lookupNode(VkStructureType sType) const noexcept
{
    auto it = std::find_if(cbegin(), cend(),
        [sType](auto& it)
        {
           return (it.getNode()->sType == sType);
        });
    if (it != cend())
        return it->getNode();
    return nullptr;
}
} // namespace magma
