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

#include "internal/pch.h"

#include "allocator/allocator.h"
#include "allocator/objectAllocator.h"

#include "objects/buffer.h"
#include "objects/bufferView.h"
#include "objects/commandBuffer.h"
#include "objects/commandPool.h"
#include "objects/computePipeline.h"
#include "objects/debugReportCallback.h"
#include "objects/descriptorPool.h"
#include "objects/descriptorSet.h"
#include "objects/descriptorSetLayout.h"
#include "objects/device.h"
#include "objects/deviceMemory.h"
#include "objects/display.h"
#include "objects/displayMode.h"
#include "objects/dstTransferBuffer.h"
#include "objects/event.h"
#include "objects/fence.h"
#include "objects/framebuffer.h"
#include "objects/graphicsPipeline.h"
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
#include "objects/rayTracingBuffer.h"
#include "objects/rayTracingPipeline.h"
#include "objects/sampler.h"
#include "objects/semaphore.h"
#include "objects/shaderModule.h"
#include "objects/srcTransferBuffer.h"
#include "objects/storageBuffer.h"
#include "objects/storageTexelBuffer.h"
#include "objects/surface.h"
#include "objects/swapchain.h"
#include "objects/uniformBuffer.h"
#include "objects/uniformTexelBuffer.h"
#include "objects/vertexBuffer.h"

#include "shaders/specialization.h"
#include "shaders/pipelineShaderStage.h"
#include "shaders/shaderStages.h"
#include "shaders/rayTracingShaderGroup.h"

#include "states/colorBlendState.h"
#include "states/colorBlendStateAdvanced.h"
#include "states/depthStencilState.h"
#include "states/inputAssemblyState.h"
#include "states/multisampleState.h"
#include "states/rasterizationState.h"
#include "states/samplerState.h"
#include "states/tesselationState.h"
#include "states/vertexInputState.h"
#include "states/vertexInputStructure.h"
#include "states/viewportState.h"

#include "descriptors/attachment.h"
#include "descriptors/bindings.h"
#include "descriptors/descriptors.h"
#include "descriptors/subpass.h"

#include "barriers/bufferMemoryBarrier.h"
#include "barriers/globalMemoryBarrier.h"
#include "barriers/imageMemoryBarrier.h"

#include "misc/clearAttachment.h"
#include "misc/clearValue.h"
#include "misc/deviceExtension.h"
#include "misc/deviceQueueDesc.h"
#include "misc/exception.h"
#include "misc/format.h"
#include "misc/imageSubresourceRange.h"
#include "misc/instanceExtension.h"
#include "misc/pushConstants.h"
#include "misc/scissor.h"
#include "misc/scopedDebugMarker.h"
#include "misc/vectorTypes.h"
#include "misc/viewport.h"

#include "extensions/instanceExtensions.h"
#include "extensions/instanceLayers.h"
#include "extensions/physicalDeviceExtensions.h"

#include "auxiliary/blitRectangle.h"
#include "auxiliary/framebuffers.h"
#include "auxiliary/immediateRender.h"
#include "auxiliary/mipmapGenerator.h"
#include "auxiliary/packedTypes.h"
#include "auxiliary/shaderCompiler.h"

#include "helpers/alignedUniformArray.h"
#include "helpers/executeCommandBuffer.h"
#include "helpers/mapScoped.h"
#include "helpers/stackArray.h"
#include "helpers/stringize.h"
#include "helpers/typedefs.h"
#include "helpers/uniformArray.h"
