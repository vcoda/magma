CC=g++
PLATFORM=VK_USE_PLATFORM_XLIB_KHR
BASE_CFLAGS=-std=c++14 -pedantic -Wall -Wextra -D$(PLATFORM) -I$(VULKAN_SDK)/include
DEBUG_CFLAGS=$(BASE_CFLAGS) -D_DEBUG
RELEASE_CFLAGS=$(BASE_CFLAGS)
DEPS=

DO_CC=$(CC) $(CFLAGS) -o $@ -c $<

DEBUG_TARGET=libmagmad.a
RELEASE_TARGET=libmagma.a

MAGMA_OBJS = \
	descriptors/attachment.o \
	descriptors/subpass.o \
	\
	misc/bufferMemoryBarrier.o \
	misc/debugMarker.o \
	misc/deviceQueueDesc.o \
	misc/exception.o \
	misc/extensions.o \
	misc/format.o \
	misc/imageMemoryBarrier.o \
	\
	objects/buffer.o \
	objects/bufferView.o \
	objects/commandBuffer.o \
	objects/commandPool.o \
	objects/debugObject.o \
	objects/debugReportCallback.o \
	objects/descriptorPool.o \
	objects/descriptorSet.o \
	objects/descriptorSetLayout.o \
	objects/device.o \
	objects/deviceMemory.o \
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
	objects/instance.o \
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
	objects/vertexBuffer.o \
    objects/uniformTexelBuffer.o \
	\
	states/colorBlendState.o \
	states/depthStencilState.o \
	states/inputAssemblyState.o \
	states/multisampleState.o \
	states/rasterizationState.o \
	states/samplerState.o \
	states/tesselationState.o \
	states/vertexInputState.o \
	states/viewportState.o

%.o: %.cpp $(DEPS)
	$(CC) $(DEBUG_CFLAGS) -o $@ -c $<

debug : $(MAGMA_OBJS)
	@echo "Make debug static library"
	@ar rcs $(DEBUG_TARGET) $(MAGMA_OBJS)

release : $(MAGMA_OBJS)
	@echo "Make release static library"
	@ar rcs $(RELEASE_TARGET) $(MAGMA_OBJS)

clean :
	@find . -name '*.a' -delete
	rm $(MAGMA_OBJS)
