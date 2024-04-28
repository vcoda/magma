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
#include "misc/structureChain.h"

#ifdef VK_KHR_external_memory
void test(const magma::StructureChain& structureChain)
{
    std::cout << "Chain length: " << structureChain.getSize() << std::endl;
    std::cout << "Hash: " << structureChain.getHash() << std::endl;
    const VkExportMemoryAllocateInfoKHR *exportMemoryAllocateNode = structureChain.findNode<VkExportMemoryAllocateInfoKHR>();
    if (!exportMemoryAllocateNode)
        std::cerr << "Node VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_KHR not found!" << std::endl;
    const VkExternalMemoryBufferCreateInfoKHR *externalMemoryBufferNode = structureChain.findNode<VkExternalMemoryBufferCreateInfoKHR>();
    if (!externalMemoryBufferNode)
        std::cerr << "Node VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO_KHR not found!" << std::endl;
    const VkExternalMemoryImageCreateInfoKHR *externalMemoryImageNode = structureChain.findNode<VkExternalMemoryImageCreateInfoKHR>();
    if (!externalMemoryImageNode)
        std::cerr << "Node VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_KHR not found!" << std::endl;
#ifdef VK_KHR_external_fence
    const VkExportFenceCreateInfoKHR *exportFenceInfo = structureChain.findNode<VkExportFenceCreateInfoKHR>();
    if (exportFenceInfo)
        std::cerr << "Node VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO_KHR should NOT be found!" << std::endl;
#endif
    const VkBaseInStructure *head = structureChain.headNode();
    if (head->sType != VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_KHR)
        std::cerr << "Invalid head node!" << std::endl;
    const VkBaseInStructure *tail = structureChain.tailNode();
    if (tail->sType != VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_KHR)
        std::cerr << "Invalid tail node!" << std::endl;
}

int main()
{
    magma::StructureChain structureChain;
    MAGMA_ASSERT(structureChain.empty());
    VkExternalMemoryImageCreateInfoKHR externalMemoryImageInfo = {};
    externalMemoryImageInfo.sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_KHR;
    VkExternalMemoryBufferCreateInfoKHR externalMemoryBufferInfo = {};
    externalMemoryBufferInfo.sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO_KHR;
    VkExportMemoryAllocateInfoKHR exportMemoryAllocateInfo = {};
    exportMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_KHR;
    structureChain.linkNode(externalMemoryImageInfo);
    structureChain.linkNode(externalMemoryBufferInfo);
    structureChain.linkNode(exportMemoryAllocateInfo);
    std::cout << "Test original instance:" << std::endl;
    test(structureChain);
    // Test chain copying
    std::cout << "Test copied instance:" << std::endl;
    magma::StructureChain copiedChain(structureChain);
    structureChain.clear();
    test(copiedChain);
    // Test chain moving
    std::cout << "Test moved instance:" << std::endl;
    magma::StructureChain movedChain(std::move(copiedChain));
    test(movedChain);
    MAGMA_ASSERT(structureChain.empty());
    MAGMA_ASSERT(copiedChain.empty());
    return 0;
}
#endif // VK_KHR_external_memory
