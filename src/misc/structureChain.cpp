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
StructureChain::StructureChain(const StructureChain& other):
    head(nullptr),
    hash(other.hash)
{
    if (other.head)
    {
        head = copyNode(other.head);
        for (auto src = other.head->pNext, dst = head;
            src; src = src->pNext, dst = dst->pNext)
        {
            dst->pNext = copyNode(src);
        }
    }
}

VkBaseOutStructure *StructureChain::tailNode() noexcept
{
    auto node = head;
    if (node)
    {
        while (node->pNext)
            node = node->pNext;
    }
    return node;
}

const VkBaseInStructure *StructureChain::tailNode() const noexcept
{
    auto node = head;
    if (node)
    {
        while (node->pNext)
            node = node->pNext;
    }
    return reinterpret_cast<const VkBaseInStructure *>(node);
}

size_t StructureChain::getSize() const noexcept
{
    if (!head)
        return 0;
    size_t size = 1;
    for (auto node = head; node->pNext; ++size)
        node = node->pNext;
    return size;
}

void StructureChain::clear() noexcept
{
    while (head)
    {
        auto node = head;
        head = node->pNext;
        node->pNext = nullptr;
        free(node);
    }
    head = nullptr;
    hash = 0ull;
}

VkBaseOutStructure *StructureChain::getNode(VkStructureType sType) noexcept
{
    for (auto node = head; node; node = node->pNext)
    {
        if (node->sType == sType)
            return node;
    }
    return nullptr;
}

const VkBaseInStructure *StructureChain::getNode(VkStructureType sType) const noexcept
{
    for (auto node = head; node; node = node->pNext)
    {
        if (node->sType == sType)
            return reinterpret_cast<const VkBaseInStructure *>(node);
    }
    return nullptr;
}

VkBaseOutStructure *StructureChain::copyNode(const VkBaseOutStructure *src) noexcept
{
    MAGMA_ASSERT(src);
    const size_t size = sizeofNode(src->sType);
    MAGMA_ASSERT(size >= sizeof(VkBaseOutStructure));
    auto dst = (VkBaseOutStructure *)malloc(size);
    MAGMA_ASSERT(dst);
    if (dst)
    {   // Copy body, clear link
        memcpy(dst, src, size);
        dst->pNext = nullptr;
    }
    return dst;
}
} // namespace magma
