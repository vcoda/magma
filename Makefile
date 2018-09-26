CC=g++
PLATFORM=VK_USE_PLATFORM_XCB_KHR
INCLUDE_DIR=-I$(VULKAN_SDK)/include
BASE_CFLAGS=-std=c++14 -m64 -msse4 -pthread -pedantic -Wall -Werror -D$(PLATFORM) $(INCLUDE_DIR)

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS=$(BASE_CFLAGS) -O0 -g -D_DEBUG
	TARGET=libmagmad.a
else
	CFLAGS=$(BASE_CFLAGS) -O3 -DNDEBUG
	TARGET=libmagma.a
endif

MAGMA_OBJS= \
	allocator/allocator.o \
	\
	barriers/bufferMemoryBarrier.o \
	barriers/globalMemoryBarrier.o \
	barriers/imageMemoryBarrier.o \
	\
	descriptors/attachment.o \
	descriptors/subpass.o \
	\
	misc/clearValue.o \
	misc/deviceQueueDesc.o \
	misc/exception.o \
	misc/extensions.o \
	misc/format.o \
	misc/scopedDebugMarker.o \
	\
	helpers/stringize.o \
	\
	objects/buffer.o \
	objects/bufferView.o \
	objects/commandBuffer.o \
	objects/commandPool.o \
	objects/conditionalRenderingBuffer.o \
	objects/debugMarker.o \
	objects/debugReportCallback.o \
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
	objects/image.o \
	objects/image1D.o \
	objects/image1DArray.o \
	objects/image2D.o \
	objects/image2DArray.o \
	objects/image3D.o \
	objects/imageCube.o \
	objects/imageView.o \
	objects/indexBuffer.o \
	objects/indirectBuffer.o \
	objects/instance.o \
	objects/object.o \
	objects/physicalDevice.o \
	objects/pipeline.o \
	objects/pipelineCache.o \
	objects/pipelineLayout.o \
	objects/queryPool.o \
	objects/queue.o \
	objects/renderPass.o \
	objects/sampler.o \
	objects/semaphore.o \
	objects/shaderModule.o \
	objects/srcTransferBuffer.o \
	objects/storageBuffer.o \
	objects/storageTexelBuffer.o \
	objects/surface.o \
	objects/swapchain.o \
	objects/uniformTexelBuffer.o \
	objects/vertexBuffer.o \
	\
	states/colorBlendState.o \
	states/depthStencilState.o \
	states/inputAssemblyState.o \
	states/multisampleState.o \
	states/rasterizationState.o \
	states/samplerState.o \
	states/tesselationState.o \
	states/vertexInputState.o \
	states/viewportState.o \
	\
	utilities/immediateRender.o

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
