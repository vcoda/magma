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
#pragma once

#include "src/core/pch.h"

#include "src/allocator/alignedAllocator.h"
#include "src/allocator/cxxAllocator.h"
#include "src/allocator/debugAlignedAllocator.h"
#include "src/allocator/deviceMemoryAllocator.h"

#include "src/objects/accelerationStructure.h"
#include "src/objects/accelerationStructureInstanceBuffer.h"
#include "src/objects/buffer.h"
#include "src/objects/bufferView.h"
#include "src/objects/commandBuffer.h"
#include "src/objects/commandPool.h"
#include "src/objects/computePipeline.h"
#include "src/objects/computePipelineBatch.h"
#include "src/objects/computePipelineLibrary.h"
#include "src/objects/debugReportCallback.h"
#include "src/objects/debugUtilsMessenger.h"
#include "src/objects/deferredOperation.h"
#include "src/objects/descriptorPool.h"
#include "src/objects/descriptorSet.h"
#include "src/objects/descriptorSetLayout.h"
#include "src/objects/device.h"
#include "src/objects/deviceMemory.h"
#include "src/objects/deviceResourcePool.h"
#include "src/objects/display.h"
#include "src/objects/displayMode.h"
#include "src/objects/dstTransferBuffer.h"
#include "src/objects/dynamicUniformBuffer.h"
#include "src/objects/event.h"
#include "src/objects/externalImage.h"
#include "src/objects/fence.h"
#include "src/objects/fragmentDensityMap.h"
#include "src/objects/fragmentShadingRateAttachment.h"
#include "src/objects/framebuffer.h"
#include "src/objects/fullScreenExclusiveSwapchain.h"
#include "src/objects/graphicsPipeline.h"
#include "src/objects/graphicsPipelineBatch.h"
#include "src/objects/graphicsPipelineLibrary.h"
#include "src/objects/image.h"
#include "src/objects/image1D.h"
#include "src/objects/image1DArray.h"
#include "src/objects/image2D.h"
#include "src/objects/image2DArray.h"
#include "src/objects/imageAttachment.h"
#include "src/objects/image3D.h"
#include "src/objects/imageCube.h"
#include "src/objects/imagelessFramebuffer.h"
#include "src/objects/imageView.h"
#include "src/objects/indexBuffer.h"
#include "src/objects/indirectBuffer.h"
#include "src/objects/instance.h"
#include "src/objects/mutableImage.h"
#include "src/objects/mutableImageView.h"
#include "src/objects/physicalDevice.h"
#include "src/objects/pipeline.h"
#include "src/objects/pipelineBatch.h"
#include "src/objects/pipelineCache.h"
#include "src/objects/pipelineLayout.h"
#include "src/objects/pipelineLibrary.h"
#include "src/objects/primaryCommandBuffer.h"
#include "src/objects/privateDataSlot.h"
#include "src/objects/queryPool.h"
#include "src/objects/queue.h"
#include "src/objects/rayTracingBuffer.h"
#include "src/objects/rayTracingPipeline.h"
#include "src/objects/rayTracingPipelineBatch.h"
#include "src/objects/renderPass.h"
#include "src/objects/renderPassMultiview.h"
#include "src/objects/resource.h"
#include "src/objects/sampler.h"
#include "src/objects/samplerYcbcrConversion.h"
#include "src/objects/secondaryCommandBuffer.h"
#include "src/objects/semaphore.h"
#include "src/objects/shaderModule.h"
#include "src/objects/srcTransferBuffer.h"
#include "src/objects/storageBuffer.h"
#include "src/objects/storageTexelBuffer.h"
#include "src/objects/surface.h"
#include "src/objects/swapchain.h"
#include "src/objects/swapchainImage.h"
#include "src/objects/transformFeedbackBuffer.h"
#include "src/objects/typedefs.h"
#include "src/objects/uniformBuffer.h"
#include "src/objects/uniformTexelBuffer.h"
#include "src/objects/validationCache.h"
#include "src/objects/vertexBuffer.h"

#include "src/shaders/pipelineShaderStage.h"
#include "src/shaders/rayTracingShaderGroup.h"
#include "src/shaders/shaderBindingTable.h"
#include "src/shaders/shaderReflection.h"
#include "src/shaders/shaderReflectionFactory.h"
#include "src/shaders/shaderStages.h"
#include "src/shaders/specialization.h"

#include "src/renderpass/attachment.h"
#include "src/renderpass/loadStoreOp.h"
#include "src/renderpass/subpass.h"

#include "src/states/colorBlendAttachmentState.h"
#include "src/states/colorBlendState.h"
#include "src/states/colorBlendStateAdvanced.h"
#include "src/states/colorLogicOpState.h"
#include "src/states/colorMask.h"
#include "src/states/conservativeRasterizationState.h"
#include "src/states/depthComparisonSamplerState.h"
#include "src/states/depthStencilState.h"
#include "src/states/inputAssemblyState.h"
#include "src/states/lineRasterizationState.h"
#include "src/states/minmaxSamplerState.h"
#include "src/states/multisampleCoverageReductionState.h"
#include "src/states/multisampleState.h"
#include "src/states/multiViewportState.h"
#include "src/states/provokingVertexState.h"
#include "src/states/rasterizationOrderState.h"
#include "src/states/rasterizationState.h"
#include "src/states/samplerState.h"
#include "src/states/samplerYcbcrConversionState.h"
#include "src/states/streamRasterizationState.h"
#include "src/states/tesselationState.h"
#include "src/states/vertexAttributeType.h"
#include "src/states/vertexFormat.h"
#include "src/states/vertexInputState.h"
#include "src/states/vertexInputStructure.h"
#include "src/states/viewportState.h"

#include "src/descriptors/accelerationStructure.h"
#include "src/descriptors/binding.h"
#include "src/descriptors/bufferDescriptor.h"
#include "src/descriptors/bufferDescriptorArray.h"
#include "src/descriptors/descriptor.h"
#include "src/descriptors/descriptorPool.h"
#include "src/descriptors/descriptorSetTable.h"
#include "src/descriptors/imageDescriptor.h"
#include "src/descriptors/imageDescriptorArray.h"
#include "src/descriptors/inlineUniformBlock.h"

#include "src/barriers/bufferMemoryBarrier.h"
#include "src/barriers/imageMemoryBarrier.h"
#include "src/barriers/memoryBarrier.h"
#include "src/barriers/subpassDependency.h"
#include "src/barriers/subpassDependency2.h"

#include "src/misc/application.h"
#include "src/misc/borderColor.h"
#include "src/misc/clearAttachment.h"
#include "src/misc/clearValue.h"
#include "src/misc/deviceFaultInfo.h"
#include "src/misc/deviceFeatures.h"
#include "src/misc/deviceQueueDescriptor.h"
#include "src/misc/extension.h"
#include "src/misc/format.h"
#include "src/misc/framebufferAttachmentImage.h"
#include "src/misc/geometry.h"
#include "src/misc/imageResolve.h"
#include "src/misc/imageSubresourceRange.h"
#include "src/misc/pipelineExecutable.h"
#include "src/misc/pushConstantRange.h"
#include "src/misc/scissor.h"
#include "src/misc/scopedDebugMarker.h"
#include "src/misc/sharing.h"
#include "src/misc/structureChain.h"
#include "src/misc/viewport.h"

#include "src/platform/androidHardwareBuffer.h"
#include "src/platform/androidSurface.h"
#include "src/platform/d3dExternalSemaphore.h"
#include "src/platform/directFbSurface.h"
#include "src/platform/externalFence.h"
#include "src/platform/externalSemaphore.h"
#include "src/platform/fuchsiaSurface.h"
#include "src/platform/iosSurface.h"
#include "src/platform/macosSurface.h"
#include "src/platform/metalSurface.h"
#include "src/platform/qnxSurface.h"
#include "src/platform/typedefs.h"
#include "src/platform/viSurface.h"
#include "src/platform/waylandSurface.h"
#include "src/platform/win32ExternalSemaphore.h"
#include "src/platform/win32Surface.h"
#include "src/platform/xcbSurface.h"
#include "src/platform/xlibSurface.h"

#include "src/extensions/deviceExtensions.h"
#include "src/extensions/instanceExtensions.h"
#include "src/extensions/instanceLayers.h"

#include "src/auxiliary/accumulationBuffer.h"
#include "src/auxiliary/baseFramebuffer.h"
#include "src/auxiliary/blitRectangle.h"
#include "src/auxiliary/colorFramebuffer.h"
#include "src/auxiliary/colorMultisampleFramebuffer.h"
#include "src/auxiliary/depthFramebuffer.h"
#include "src/auxiliary/fillRectangleVertexShader.h"
#include "src/auxiliary/frameGrabber.h"
#include "src/auxiliary/generateMipmap.h"
#include "src/auxiliary/graphicsPipelineCache.h"
#include "src/auxiliary/immediateRender.h"
#include "src/auxiliary/multiAttachmentFramebuffer.h"
#include "src/auxiliary/profiler.h"
#include "src/auxiliary/shaderCompiler.h"
#include "src/auxiliary/sprite.h"
#include "src/auxiliary/swapchainFramebuffer.h"
#include "src/auxiliary/textShader.h"
#include "src/auxiliary/typedefs.h"

#include "src/helpers/alignedUniformArray.h"
#include "src/helpers/enumerationCast.h"
#include "src/helpers/executeCommandBuffer.h"
#include "src/helpers/executeDeferredOperation.h"
#include "src/helpers/mapScoped.h"
#include "src/helpers/mapTyped.h"
#include "src/helpers/stackArray.h"
#include "src/helpers/streamInsertOperators.h"
#include "src/helpers/stringifyFlags.h"
#include "src/helpers/uniformArray.h"

#include "src/packed/color.h"
#include "src/packed/vector.h"

#include "src/exceptions/compileError.h"
#include "src/exceptions/errorResult.h"
#include "src/exceptions/notImplemented.h"
#include "src/exceptions/reflectionErrorResult.h"
#include "src/exceptions/unsupportedExtension.h"
