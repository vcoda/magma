# Magma - Abstraction layer over Khronos Vulkan API.
# Copyright (C) 2018-2024 Victor Coda.

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

.DEFAULT_GOAL := magma

PCH_HEADER=src/core/pch.h
SRC_OBJS= \
	src/magma.o \
	\
	src/allocator/alignedAllocator.o \
	src/allocator/allocationCallbacks.o \
	src/allocator/debugAlignedAllocator.o \
	src/allocator/deviceMemoryAllocator.o \
	src/allocator/vmaImplementation.o \
	\
	src/auxiliary/accumulationBuffer.o \
	src/auxiliary/auxframebuffer.o \
	src/auxiliary/blitRectangle.o \
	src/auxiliary/fillRectangleVertexShader.o \
	src/auxiliary/frameGrabber.o \
	src/auxiliary/generateMipmap.o \
	src/auxiliary/graphicsPipelineCache.o \
	src/auxiliary/imageDescriptorSet.o \
	src/auxiliary/immediateRender.o \
	src/auxiliary/profiler.o \
	src/auxiliary/shaderCompiler.o \
	src/auxiliary/sprite.o \
	src/auxiliary/textShader.o \
	\
	src/barriers/imageMemoryBarrier.o \
	\
	src/descriptors/accelerationStructureDescriptor.o \
	src/descriptors/bufferDescriptor.o \
	src/descriptors/imageDescriptor.o \
	\
	src/exceptions/errorResult.o \
	src/exceptions/exception.o \
	src/exceptions/reflectionErrorResult.o \
	\
	src/extensions/deviceExtensions.o \
	src/extensions/instanceExtensions.o \
	src/extensions/instanceLayers.o \
	\
	src/helpers/executeCommandBuffer.o \
	src/helpers/executeDeferredOperation.o \
	src/helpers/layoutTransition.o \
	src/helpers/streamInsertOperators.o \
	src/helpers/stringifyFlags.o \
	\
	src/misc/application.o \
	src/misc/deviceFaultInfo.o \
	src/misc/deviceFeatures.o \
	src/misc/deviceQueueDescriptor.o \
	src/misc/featureQuery.o \
	src/misc/finish.o \
	src/misc/format.o \
	src/misc/framebufferAttachmentImage.o \
	src/misc/pipelineExecutable.o \
	src/misc/scopedDebugMarker.o \
	src/misc/sharing.o \
	src/misc/structureChain.o \
	\
	src/objects/model/deviceChild.o \
	src/objects/accelerationStructure.o \
	src/objects/accelerationStructureInputBuffer.o \
	src/objects/baseDeviceMemory.o \
	src/objects/baseUniformBuffer.o \
	src/objects/bottomLevelStructure.o \
	src/objects/buffer.o \
	src/objects/bufferView.o \
	src/objects/commandBuffer.o \
	src/objects/commandBufferDebug.o \
	src/objects/commandPool.o \
	src/objects/computePipeline.o \
	src/objects/computePipelineBatch.o \
	src/objects/computePipelineLibrary.o \
	src/objects/conditionalRenderingBuffer.o \
	src/objects/countBuffer.o \
	src/objects/debugReportCallback.o \
	src/objects/debugUtilsMessenger.o \
	src/objects/deferredOperation.o \
	src/objects/descriptorPool.o \
	src/objects/descriptorSet.o \
	src/objects/descriptorSetLayout.o \
	src/objects/device.o \
	src/objects/deviceMemory.o \
	src/objects/deviceResourcePool.o \
	src/objects/display.o \
	src/objects/displayMode.o \
	src/objects/dstTransferBuffer.o \
	src/objects/dynamicUniformBuffer.o \
	src/objects/event.o \
	src/objects/externalImage.o \
	src/objects/fence.o \
	src/objects/fragmentDensityMap.o \
	src/objects/fragmentDensityMapView.o \
	src/objects/fragmentShadingRateAttachment.o \
	src/objects/framebuffer.o \
	src/objects/fullScreenExclusiveSwapchain.o \
	src/objects/graphicsPipeline.o \
	src/objects/graphicsPipelineBatch.o \
	src/objects/graphicsPipelineLibrary.o \
	src/objects/image.o \
	src/objects/image1D.o \
	src/objects/image1DArray.o \
	src/objects/image2D.o \
	src/objects/image2DArray.o \
	src/objects/image3D.o \
	src/objects/imageAttachment.o \
	src/objects/imageCube.o \
	src/objects/imagelessFramebuffer.o \
	src/objects/imageView.o \
	src/objects/indexBuffer.o \
	src/objects/indirectBuffer.o \
	src/objects/instance.o \
	src/objects/leanCommandBuffer.o \
	src/objects/managedDeviceMemory.o \
	src/objects/multiviewRenderPass.o \
	src/objects/mutableImage.o \
	src/objects/mutableImageView.o \
	src/objects/physicalDevice.o \
	src/objects/physicalDeviceFeatures.o \
	src/objects/physicalDeviceGroup.o \
	src/objects/pipeline.o \
	src/objects/pipelineBatch.o \
	src/objects/pipelineCache.o \
	src/objects/pipelineLayout.o \
	src/objects/pipelineLibrary.o \
	src/objects/primaryCommandBuffer.o \
	src/objects/privateDataSlot.o \
	src/objects/queryPool.o \
	src/objects/queue.o \
	src/objects/rayTracingPipeline.o \
	src/objects/rayTracingPipelineBatch.o \
	src/objects/renderPass.o \
	src/objects/sampler.o \
	src/objects/samplerYcbcrConversion.o \
	src/objects/semaphore.o \
	src/objects/shaderModule.o \
	src/objects/srcTransferBuffer.o \
	src/objects/storageBuffer.o \
	src/objects/storageTexelBuffer.o \
	src/objects/surface.o \
	src/objects/swapchain.o \
	src/objects/swapchainImage.o \
	src/objects/timelineSemaphore.o \
	src/objects/topLevelStructure.o \
	src/objects/transformFeedbackBuffer.o \
	src/objects/uniformTexelBuffer.o \
	src/objects/validationCache.o \
	src/objects/variableCountDescriptorSet.o \
	src/objects/vertexBuffer.o \
	\
	src/platform/directFbSurface.o \
	src/platform/externalFence.o \
	src/platform/externalSemaphore.o \
	src/platform/qnxSurface.o \
	src/platform/waylandSurface.o \
	src/platform/xcbSurface.o \
	src/platform/xlibSurface.o \
	\
	src/raytracing/accelerationStructureGeometry.o \
	\
	src/renderpass/subpass.o \
	\
	src/shaders/pipelineShaderStage.o \
	src/shaders/shaderBindingTable.o \
	src/shaders/shaderGroup.o \
	src/shaders/shaderReflection.o \
	src/shaders/specialization.o \
	\
	src/states/colorBlendStateAdvanced.o \
	src/states/formattedOutput.o \
	src/states/multiColorBlendState.o \
	src/states/multisampleState.o \
	src/states/multiViewportState.o \
	src/states/rasterizationState.o \
	src/states/vertexInputState.o \
	src/states/vertexInputStructure.o \
	src/states/viewportState.o \
	\
	src/third-party/SPIRV-Reflect/spirv_reflect.o

CC=g++
GLSLC=$(VULKAN_SDK)/bin/glslangValidator

PLATFORM=VK_USE_PLATFORM_XCB_KHR

INCLUDE_DIR=-I$(VULKAN_SDK)/include -Isrc/core
GLSL_DIR=src/auxiliary/spirv
SPIRV_DIR=$(GLSL_DIR)/output

CONSTEXPR_DEPTH_FLAGS=-ftemplate-depth=2048 -fconstexpr-depth=2048
BASE_CFLAGS=-std=c++17 -m64 -msse4 -pthread $(CONSTEXPR_DEPTH_FLAGS) -Wno-enum-compare -D$(PLATFORM) $(INCLUDE_DIR)

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS=$(BASE_CFLAGS) -O0 -g -D_DEBUG
	BUILD_TARGET=libmagmad.a
else
	CFLAGS=$(BASE_CFLAGS) -O3 -DNDEBUG
	BUILD_TARGET=libmagma.a
endif

PCH=$(PCH_HEADER).gch
DEPS := $(SRC_OBJS:.o=.d)

-include $(DEPS)

%.o: %.cpp
	$(CC) $(CFLAGS) -include $(PCH_HEADER) -c $< -o $@

$(PCH): $(PCH_HEADER)
	$(CC) $(CFLAGS) -o $@ $<

shaders:
	@echo "Compiling shaders"
	@rm -rf $(SPIRV_DIR)
	@mkdir $(SPIRV_DIR)
	$(GLSLC) -V --vn fsBlit $(GLSL_DIR)/blit.frag -o $(SPIRV_DIR)/blitf
	$(GLSLC) -V --vn vsBlit $(GLSL_DIR)/blit.vert -o $(SPIRV_DIR)/blitv
	$(GLSLC) -V -DNV --vn vsBlitNV $(GLSL_DIR)/blit.vert -o $(SPIRV_DIR)/blitv_nv
	$(GLSLC) -V --vn fsFont $(GLSL_DIR)/font.frag -o $(SPIRV_DIR)/fontf
	$(GLSLC) -V --vn vsImm $(GLSL_DIR)/imm.vert -o $(SPIRV_DIR)/immv
	$(GLSLC) -V --vn fsImm $(GLSL_DIR)/imm.frag -o $(SPIRV_DIR)/immf

build: $(PCH) $(SRC_OBJS)
	@echo "Make" $(BUILD_TARGET)
	@ar rcs $(BUILD_TARGET) $(SRC_OBJS)

magma:
	$(MAKE) shaders
	$(MAKE) build

clean:
	@find . -iregex '.*\.\(o\|a\)' -delete
	@rm -rf $(PCH)
	@rm -rf $(DEPS)
	@rm -rf $(SPIRV_DIR)
