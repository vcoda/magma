# Magma - Abstraction layer over Khronos Vulkan API.
# Copyright (C) 2018-2023 Victor Coda.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

CC=g++
PLATFORM=VK_USE_PLATFORM_XCB_KHR
INCLUDE_DIR=-I$(VULKAN_SDK)/include -Icore
CONSTEXPR_DEPTH_FLAGS=-ftemplate-depth=2048 -fconstexpr-depth=2048
BASE_CFLAGS=-std=c++14 -m64 -msse4 -pthread $(CONSTEXPR_DEPTH_FLAGS) -Wno-enum-compare -Werror -D$(PLATFORM) $(INCLUDE_DIR)

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS=$(BASE_CFLAGS) -O0 -g -D_DEBUG
	TARGET=libmagmad.a
else
	CFLAGS=$(BASE_CFLAGS) -O3 -DNDEBUG
	TARGET=libmagma.a
endif

MAGMA_OBJS= \
	magma.o \
	\
	allocator/alignedAllocator.o \
	allocator/allocationCallbacks.o \
	allocator/cxxAllocator.o \
	allocator/debugAlignedAllocator.o \
	allocator/deviceMemoryAllocator.o \
	allocator/vmaImplementation.o \
	\
	auxiliary/accumulationBuffer.o \
	auxiliary/baseFramebuffer.o \
	auxiliary/blitRectangle.o \
	auxiliary/colorFramebuffer.o \
	auxiliary/colorMultisampleFramebuffer.o \
	auxiliary/depthFramebuffer.o \
	auxiliary/fillRectangleVertexShader.o \
	auxiliary/frameGrabber.o \
	auxiliary/generateMipmap.o \
	auxiliary/graphicsPipelineCache.o \
	auxiliary/imageDescriptorSet.o \
	auxiliary/immediateRender.o \
	auxiliary/multiAttachmentFramebuffer.o \
	auxiliary/profiler.o \
	auxiliary/sprite.o \
	auxiliary/swapchainFramebuffer.o \
	auxiliary/textShader.o \
	\
	barriers/bufferMemoryBarrier.o \
	barriers/imageMemoryBarrier.o \
	\
	core/pch.o \
	\
	descriptors/accelerationStructureDescriptor.o \
	descriptors/binding.o \
	\
	exceptions/exception.o \
	\
	extensions/extensions.o \
	extensions/instanceExtensions.o \
	extensions/instanceLayers.o \
	extensions/layers.o \
	extensions/physicalDeviceExtensions.o \
	\
	helpers/checkFeatureSupport.o \
	helpers/executeCommandBuffer.o \
	helpers/executeDeferredOperation.o \
	helpers/stringize.o \
	\
	misc/application.o \
	misc/deviceFaultInfo.o \
	misc/deviceFeatures.o \
	misc/deviceQueueDescriptor.o \
	misc/format.o \
	misc/framebufferAttachmentImage.o \
	misc/geometry.o \
	misc/pipelineExecutable.o \
	misc/scopedDebugMarker.o \
	misc/sharing.o \
	misc/structureChain.o \
	\
	objects/accelerationStructure.o \
	objects/accelerationStructureInstanceBuffer.o \
	objects/accelerationStructureStorageBuffer.o \
	objects/baseDeviceMemory.o \
	objects/buffer.o \
	objects/bufferView.o \
	objects/commandBuffer.o \
	objects/commandBufferDebug.o \
	objects/commandPool.o \
	objects/computePipeline.o \
	objects/computePipelines.o \
	objects/conditionalRenderingBuffer.o \
	objects/debugReportCallback.o \
	objects/debugUtilsMessenger.o \
	objects/deferredOperation.o \
	objects/descriptorPool.o \
	objects/descriptorSet.o \
	objects/descriptorSetLayout.o \
	objects/device.o \
	objects/deviceMemory.o \
	objects/display.o \
	objects/displayMode.o \
	objects/dstTransferBuffer.o \
	objects/event.o \
	objects/fence.o \
	objects/fragmentDensityMap.o \
	objects/fragmentDensityMapView.o \
	objects/fragmentShadingRateAttachment.o \
	objects/framebuffer.o \
	objects/fullScreenExclusiveSwapchain.o \
	objects/graphicsPipeline.o \
	objects/graphicsPipelines.o \
	objects/image.o \
	objects/image1D.o \
	objects/image1DArray.o \
	objects/image2D.o \
	objects/image2DArray.o \
	objects/image3D.o \
	objects/imageAttachment.o \
	objects/imageCube.o \
	objects/imagelessFramebuffer.o \
	objects/imageView.o \
	objects/indexBuffer.o \
	objects/indirectBuffer.o \
	objects/instance.o \
	objects/managedDeviceMemory.o \
	objects/mutableImage.o \
	objects/mutableImageView.o \
	objects/object.o \
	objects/physicalDevice.o \
	objects/physicalDeviceGroup.o \
	objects/pipeline.o \
	objects/pipelineCache.o \
	objects/pipelineLayout.o \
	objects/primaryCommandBuffer.o \
	objects/privateDataSlot.o \
	objects/queryPool.o \
	objects/queue.o \
	objects/rayTracingBuffer.o \
	objects/rayTracingPipeline.o \
	objects/rayTracingPipelines.o \
	objects/renderPass.o \
	objects/renderPassMultiview.o \
	objects/resource.o \
	objects/resourcePool.o \
	objects/sampler.o \
	objects/samplerYcbcrConversion.o \
	objects/semaphore.o \
	objects/shaderModule.o \
	objects/srcTransferBuffer.o \
	objects/storageBuffer.o \
	objects/storageTexelBuffer.o \
	objects/surface.o \
	objects/swapchain.o \
	objects/swapchainImage.o \
	objects/timelineSemaphore.o \
	objects/transformFeedbackBuffer.o \
	objects/uniformTexelBuffer.o \
	objects/validationCache.o \
	objects/vertexBuffer.o \
	\
	renderpass/subpass.o \
	\
	shaders/pipelineShaderStage.o \
	shaders/rayTracingShaderGroup.o \
	shaders/shaderBindingTable.o \
	shaders/shaderReflection.o \
	shaders/specialization.o \
	\
	states/colorBlendState.o \
	states/colorBlendStateAdvanced.o \
	states/multisampleState.o \
	states/multiViewportState.o \
	states/rasterizationState.o \
	states/vertexInputState.o \
	states/vertexInputStructure.o \
	states/viewportState.o \
	\
	third-party/SPIRV-Reflect/spirv_reflect.o

DEPS := $(MAGMA_OBJS:.o=.d)

-include $(DEPS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

magma: $(MAGMA_OBJS)
	@echo "Make" $(TARGET)
	@ar rcs $(TARGET) $(MAGMA_OBJS)

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete
	@rm -rf $(DEPS)
