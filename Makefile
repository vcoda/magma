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
	allocator/deviceMemoryAllocator.o \
	\
	auxiliary/accumulationBuffer.o \
	auxiliary/baseFramebuffer.o \
	auxiliary/blitRectangle.o \
	auxiliary/colorFramebuffer.o \
	auxiliary/colorMultisampleFramebuffer.o \
	auxiliary/colorMultisampleReadFramebuffer.o \
	auxiliary/depthFramebuffer.o \
	auxiliary/fillRectangleVertexShader.o \
	auxiliary/frameGrabber.o \
	auxiliary/generateMipmap.o \
	auxiliary/graphicsPipelineCache.o \
	auxiliary/imageDescriptorSet.o \
	auxiliary/immediateRender.o \
	auxiliary/sprite.o \
	auxiliary/swapchainFramebuffer.o \
	auxiliary/textShader.o \
	\
	barriers/bufferMemoryBarrier.o \
	barriers/imageMemoryBarrier.o \
	\
	core/pch.o \
	\
	descriptors/binding.o \
	descriptors/bindingArray.o \
	descriptors/subpass.o \
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
	helpers/stringize.o \
	\
	misc/deviceQueueDescriptor.o \
	misc/format.o \
	misc/geometry.o \
	misc/scopedDebugMarker.o \
	\
	objects/accelerationStructure.o \
	objects/accelerationStructureInstanceBuffer.o \
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
	objects/framebuffer.o \
	objects/graphicsPipeline.o \
	objects/graphicsPipelines.o \
	objects/image.o \
	objects/image1D.o \
	objects/image1DArray.o \
	objects/image2D.o \
	objects/image2DArray.o \
	objects/image2DAttachment.o \
	objects/image3D.o \
	objects/imageCube.o \
	objects/imagelessFramebuffer.o \
	objects/imageView.o \
	objects/indexBuffer.o \
	objects/indirectBuffer.o \
	objects/instance.o \
	objects/object.o \
	objects/physicalDevice.o \
	objects/physicalDeviceGroup.o \
	objects/pipeline.o \
	objects/pipelineCache.o \
	objects/pipelineLayout.o \
	objects/primaryCommandBuffer.o \
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
	objects/swapchainFullScreenExclusive.o \
	objects/transformFeedbackBuffer.o \
	objects/uniformTexelBuffer.o \
	objects/validationCache.o \
	objects/vertexBuffer.o \
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
