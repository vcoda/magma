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
#include "../objects/commandPool.h"
#include "../objects/commandBuffer.h"
#include "../objects/fence.h"
#include "../objects/stagingBuffer.h"
#include "../objects/queue.h"

namespace magma
{
    namespace helpers
    {
        // Data transfer from host visible memory to device local memory
        class TransferQueue
        {
        public:
            TransferQueue(std::shared_ptr<Device> device):
                device(device),
                queue(device->getQueue(VK_QUEUE_TRANSFER_BIT, 0)) 
            {
                cmdPool.reset(new magma::CommandPool(device, queue->getFamilyIndex()));
                cmdBuffer = cmdPool->allocateCommandBuffer(true);
                fence.reset(new Fence(device));
            }

            template <typename Type>
            std::shared_ptr<LocalVertexBuffer> copyVertices(const std::vector<Type>& data)
            {
                std::shared_ptr<LocalVertexBuffer> buffer(new LocalVertexBuffer(device, data.size() * sizeof(Type)));
                copyStaged(buffer, data);
                return buffer;
            }

            template <typename IndexType>
            std::shared_ptr<LocalIndexBuffer> copyIndices(const std::vector<IndexType>& data)
            {
                std::shared_ptr<LocalIndexBuffer> buffer(new LocalIndexBuffer(device, data.size() * sizeof(IndexType),
                    sizeof(IndexType) == sizeof(uint16_t) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32));
                copyStaged(buffer, data);
                return buffer;
            }

            template <typename Type>
            void copyStaged(std::shared_ptr<Buffer> dstBuffer, const std::vector<Type>& data)
            {
                // TODO: do not create/destroy staging buffer every time, reuse when possible!
                std::shared_ptr<StagingBuffer> srcBuffer(new StagingBuffer(device, data));
                cmdBuffer->begin();
                cmdBuffer->copyBuffer(srcBuffer, dstBuffer);
                cmdBuffer->end();
                fence->reset();
                queue->submit(cmdBuffer, 0, nullptr, nullptr, fence);
                fence->wait();
            }

        private:
            std::shared_ptr<Device> device;
            std::shared_ptr<Queue> queue;
            std::shared_ptr<CommandPool> cmdPool;
            std::shared_ptr<CommandBuffer> cmdBuffer;
            std::shared_ptr<Fence> fence;
        };
    } // namespace helpers
} // namespace magma