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

#include "objects/buffer.h"
#include "objects/bufferView.h"
#include "objects/commandBuffer.h" 
#include "objects/commandPool.h"
#include "objects/debugReportCallback.h"
#include "objects/descriptorPool.h"
#include "objects/descriptorSet.h"
#include "objects/descriptorSetLayout.h"
#include "objects/device.h"
#include "objects/deviceMemory.h"
#include "objects/event.h"
#include "objects/fence.h"
#include "objects/framebuffer.h"
#include "objects/image.h"
#include "objects/image1D.h"
#include "objects/image1DArray.h"
#include "objects/image2D.h"
#include "objects/image2DArray.h"
#include "objects/image3D.h"
#include "objects/imageCube.h"
#include "objects/imageView.h"
#include "objects/indexBuffer.h"
#include "objects/indirectBuffer.h"
#include "objects/instance.h"
#include "objects/physicalDevice.h"
#include "objects/pipeline.h"
#include "objects/pipelineCache.h"
#include "objects/pipelineLayout.h"
#include "objects/queryPool.h"  
#include "objects/queue.h"   
#include "objects/renderPass.h"
#include "objects/sampler.h"
#include "objects/semaphore.h"
#include "objects/shaderModule.h"
#include "objects/storageBuffer.h"
#include "objects/surface.h"
#include "objects/swapchain.h"
#include "objects/transferBuffer.h"
#include "objects/uniformBuffer.h"
#include "objects/vertexBuffer.h"

#include "states/colorBlendState.h"
#include "states/depthStencilState.h"
#include "states/inputAssemblyState.h"
#include "states/multisampleState.h"
#include "states/rasterizationState.h"
#include "states/samplerState.h"
#include "states/tesselationState.h"
#include "states/vertexInputState.h"
#include "states/viewportState.h"

#include "descriptors/attachment.h"
#include "descriptors/bindings.h"
#include "descriptors/descriptors.h"
#include "descriptors/subpass.h"
                            
#include "misc/bufferMemoryBarrier.h"
#include "misc/clearAttachment.h"
#include "misc/clearValue.h"
#include "misc/debugMarker.h"
#include "misc/deviceQueueDesc.h"
#include "misc/exception.h"
#include "misc/extensions.h"
#include "misc/format.h"
#include "misc/imageMemoryBarrier.h"
#include "misc/pushConstants.h"
#include "misc/scissor.h"
#include "misc/viewport.h"

#include "helpers/alignedMemcpy.h"
#include "helpers/immediateRender.h"
#include "helpers/mapScoped.h"
#include "helpers/stackArray.h"

#include "typedefs.h"
