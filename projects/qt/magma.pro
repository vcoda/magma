QT -= gui

TARGET = magma
TEMPLATE = lib
CONFIG += staticlib
CONFIG += precompile_header

PRECOMPILED_HEADER = ../../src/core/pch.h

SOURCES += \
    ../../src/allocator/alignedAllocator.cpp \
    ../../src/allocator/allocationCallbacks.cpp \
    ../../src/allocator/debugAlignedAllocator.cpp \
    ../../src/allocator/deviceMemoryAllocator.cpp \
    ../../src/auxiliary/accumulationBuffer.cpp \
    ../../src/auxiliary/auxframebuffer.cpp \
    ../../src/auxiliary/blitRectangle.cpp \
    ../../src/auxiliary/fillRectangleVertexShader.cpp \
    ../../src/auxiliary/frameGrabber.cpp \
    ../../src/auxiliary/generateMipmap.cpp \
    ../../src/auxiliary/graphicsPipelineCache.cpp \
    ../../src/auxiliary/imageDescriptorSet.cpp \
    ../../src/auxiliary/immediateRender.cpp \
    ../../src/auxiliary/profiler.cpp \
    ../../src/auxiliary/shaderCompiler.cpp \
    ../../src/auxiliary/sprite.cpp \
    ../../src/auxiliary/textShader.cpp \
    ../../src/barriers/imageMemoryBarrier.cpp \
    ../../src/core/pch.cpp \
    ../../src/descriptors/accelerationStructureDescriptor.cpp \
    ../../src/descriptors/bufferDescriptor.cpp \
    ../../src/descriptors/imageDescriptor.cpp \
    ../../src/exceptions/errorHandler.cpp \
    ../../src/exceptions/errorResult.cpp \
    ../../src/exceptions/reflectionError.cpp \
    ../../src/exceptions/spirvError.cpp \
    ../../src/extensions/deviceExtensions.cpp \
    ../../src/extensions/deviceLayers.cpp \
    ../../src/extensions/instanceExtensions.cpp \
    ../../src/extensions/instanceLayers.cpp \
    ../../src/helpers/executeCommandBuffer.cpp \
    ../../src/helpers/executeDeferredOperation.cpp \
    ../../src/helpers/layoutTransition.cpp \
    ../../src/helpers/streamInsertOperators.cpp \
    ../../src/helpers/stringifyFlags.cpp \
    ../../src/misc/application.cpp \
    ../../src/misc/deviceFaultInfo.cpp \
    ../../src/misc/deviceFeatures.cpp \
    ../../src/misc/deviceQueueDescriptor.cpp \
    ../../src/misc/featureQuery.cpp \
    ../../src/misc/finish.cpp \
    ../../src/misc/format.cpp \
    ../../src/misc/framebufferAttachmentImage.cpp \
    ../../src/misc/pipelineExecutable.cpp \
    ../../src/misc/scopedDebugMarker.cpp \
    ../../src/misc/sharing.cpp \
    ../../src/misc/structureChain.cpp \
    ../../src/objects/model/deviceChild.cpp \
    ../../src/objects/accelerationStructure.cpp \
    ../../src/objects/accelerationStructureInputBuffer.cpp \
    ../../src/objects/baseCountBuffer.cpp \
    ../../src/objects/baseDeviceMemory.cpp \
    ../../src/objects/baseUniformBuffer.cpp \
    ../../src/objects/bottomLevelStructure.cpp \
    ../../src/objects/buffer.cpp \
    ../../src/objects/bufferView.cpp \
    ../../src/objects/commandBuffer.cpp \
    ../../src/objects/commandBufferDebug.cpp \
    ../../src/objects/commandPool.cpp \
    ../../src/objects/computePipeline.cpp \
    ../../src/objects/computePipelineBatch.cpp \
    ../../src/objects/computePipelineLibrary.cpp \
    ../../src/objects/conditionalRenderingBuffer.cpp \
    ../../src/objects/countBuffer.cpp \
    ../../src/objects/debugReportCallback.cpp \
    ../../src/objects/debugUtilsMessenger.cpp \
    ../../src/objects/deferredOperation.cpp \
    ../../src/objects/descriptorPool.cpp \
    ../../src/objects/descriptorSet.cpp \
    ../../src/objects/descriptorSetLayout.cpp \
    ../../src/objects/device.cpp \
    ../../src/objects/deviceMemory.cpp \
    ../../src/objects/deviceResourcePool.cpp \
    ../../src/objects/display.cpp \
    ../../src/objects/displayMode.cpp \
    ../../src/objects/dstTransferBuffer.cpp \
    ../../src/objects/dynamicUniformBuffer.cpp \
    ../../src/objects/event.cpp \
    ../../src/objects/externalImage.cpp \
    ../../src/objects/fence.cpp \
    ../../src/objects/fragmentDensityMap.cpp \
    ../../src/objects/fragmentDensityMapView.cpp \
    ../../src/objects/fragmentShadingRateAttachment.cpp \
    ../../src/objects/framebuffer.cpp \
    ../../src/objects/fullScreenExclusiveSwapchain.cpp \
    ../../src/objects/graphicsPipeline.cpp \
    ../../src/objects/graphicsPipelineBatch.cpp \
    ../../src/objects/graphicsPipelineLibrary.cpp \
    ../../src/objects/image.cpp \
    ../../src/objects/image1D.cpp \
    ../../src/objects/image1DArray.cpp \
    ../../src/objects/image2D.cpp \
    ../../src/objects/image2DArray.cpp \
    ../../src/objects/image3D.cpp \
    ../../src/objects/imageAttachment.cpp \
    ../../src/objects/imageCube.cpp \
    ../../src/objects/imagelessFramebuffer.cpp \
    ../../src/objects/imageView.cpp \
    ../../src/objects/indexBuffer.cpp \
    ../../src/objects/indirectBuffer.cpp \
    ../../src/objects/instance.cpp \
    ../../src/objects/leanCommandBuffer.cpp \
    ../../src/objects/managedDeviceMemory.cpp \
    ../../src/objects/multiviewRenderPass.cpp \
    ../../src/objects/mutableImage.cpp \
    ../../src/objects/mutableImageView.cpp \
    ../../src/objects/physicalDevice.cpp \
    ../../src/objects/physicalDeviceFeatures.cpp \
    ../../src/objects/physicalDeviceGroup.cpp \
    ../../src/objects/pipeline.cpp \
    ../../src/objects/pipelineBatch.cpp \
    ../../src/objects/pipelineCache.cpp \
    ../../src/objects/pipelineLayout.cpp \
    ../../src/objects/pipelineLibrary.cpp \
    ../../src/objects/primaryCommandBuffer.cpp \
    ../../src/objects/privateDataSlot.cpp \
    ../../src/objects/queryPool.cpp \
    ../../src/objects/queue.cpp \
    ../../src/objects/rayTracingPipeline.cpp \
    ../../src/objects/rayTracingPipelineBatch.cpp \
    ../../src/objects/renderPass.cpp \
    ../../src/objects/resource.cpp \
    ../../src/objects/sampler.cpp \
    ../../src/objects/samplerYcbcrConversion.cpp \
    ../../src/objects/semaphore.cpp \
    ../../src/objects/shaderModule.cpp \
    ../../src/objects/srcTransferBuffer.cpp \
    ../../src/objects/storageBuffer.cpp \
    ../../src/objects/storageTexelBuffer.cpp \
    ../../src/objects/surface.cpp \
    ../../src/objects/swapchain.cpp \
    ../../src/objects/swapchainImage.cpp \
    ../../src/objects/timelineSemaphore.cpp \
    ../../src/objects/topLevelStructure.cpp \
    ../../src/objects/transformFeedbackBuffer.cpp \
    ../../src/objects/uniformTexelBuffer.cpp \
    ../../src/objects/validationCache.cpp \
    ../../src/objects/variableCountDescriptorSet.cpp \
    ../../src/objects/vertexBuffer.cpp \
    ../../src/platform/androidHardwareBuffer.cpp \
    ../../src/platform/androidSurface.cpp \
    ../../src/platform/d3dExternalSemaphore.cpp \
    ../../src/platform/directFbSurface.cpp \
    ../../src/platform/externalFence.cpp \
    ../../src/platform/externalSemaphore.cpp \
    ../../src/platform/fuchsiaSurface.cpp \
    ../../src/platform/iosSurface.cpp \
    ../../src/platform/macosSurface.cpp \
    ../../src/platform/metalSurface.cpp \
    ../../src/platform/qnxSurface.cpp \
    ../../src/platform/viSurface.cpp \
    ../../src/platform/waylandSurface.cpp \
    ../../src/platform/win32ExternalSemaphore.cpp \
    ../../src/platform/win32Surface.cpp \
    ../../src/platform/xcbSurface.cpp \
    ../../src/platform/xlibSurface.cpp \
    ../../src/raytracing/accelerationStructureGeometry.cpp \
    ../../src/renderpass/subpass.cpp \
    ../../src/shaders/pipelineShaderStage.cpp \
    ../../src/shaders/shaderBindingTable.cpp \
    ../../src/shaders/shaderGroup.cpp \
    ../../src/shaders/shaderReflection.cpp \
    ../../src/shaders/specialization.cpp \
    ../../src/states/colorBlendStateAdvanced.cpp \
    ../../src/states/formattedOutput.cpp \
    ../../src/states/multiColorBlendState.cpp \
    ../../src/states/multisampleState.cpp \
    ../../src/states/multiViewportState.cpp \
    ../../src/states/rasterizationState.cpp \
    ../../src/states/vertexInputState.cpp \
    ../../src/states/vertexInputStructure.cpp \
    ../../src/states/viewportState.cpp \
    ../../src/magma.cpp \
    ../../src/third-party/SPIRV-Reflect/spirv_reflect.cpp

HEADERS += \
    ../../src/allocator/alignedAllocator.h \
    ../../src/allocator/allocator.h \
    ../../src/allocator/debugAlignedAllocator.h \
    ../../src/allocator/deviceMemoryAllocator.h \
    ../../src/auxiliary/accumulationBuffer.h \
    ../../src/auxiliary/blitRectangle.h \
    ../../src/auxiliary/fillRectangleVertexShader.h \
    ../../src/auxiliary/framebuffer.h \
    ../../src/auxiliary/frameGrabber.h \
    ../../src/auxiliary/generateMipmap.h \
    ../../src/auxiliary/graphicsPipelineCache.h \
    ../../src/auxiliary/imageDescriptorSet.h \
    ../../src/auxiliary/immediateRender.h \
    ../../src/auxiliary/profiler.h \
    ../../src/auxiliary/shaderCompiler.h \
    ../../src/auxiliary/sprite.h \
    ../../src/auxiliary/textShader.h \
    ../../src/auxiliary/typedefs.h \
    ../../src/barriers/bufferMemoryBarrier.h \
    ../../src/barriers/imageMemoryBarrier.h \
    ../../src/barriers/memoryBarrier.h \
    ../../src/barriers/subpassDependency.h \
    ../../src/barriers/subpassDependency2.h \
    ../../src/core/hashing/constexpr.h \
    ../../src/core/hashing/fnv1.h \
    ../../src/core/hashing/fnv1string.h \
    ../../src/core/core.h \
    ../../src/core/defines.h \
    ../../src/core/lentPtr.h \
    ../../src/core/macros.h \
    ../../src/core/pch.h \
    ../../src/core/popcount.h \
    ../../src/core/refCountChecker.h \
    ../../src/core/spinlock.h \
    ../../src/core/variantPtr.h \
    ../../src/descriptors/accelerationStructure.h \
    ../../src/descriptors/arrayElement.h \
    ../../src/descriptors/bufferArrayElement.h \
    ../../src/descriptors/bufferDescriptor.h \
    ../../src/descriptors/bufferDescriptorArray.h \
    ../../src/descriptors/descriptorPool.h \
    ../../src/descriptors/descriptorSetLayoutBinding.h \
    ../../src/descriptors/imageArrayElement.h \
    ../../src/descriptors/imageDescriptor.h \
    ../../src/descriptors/imageDescriptorArray.h \
    ../../src/descriptors/imageDescriptorVariableArray.h \
    ../../src/descriptors/inlineUniformBlock.h \
    ../../src/exceptions/compileError.h \
    ../../src/exceptions/errorHandler.h \
    ../../src/exceptions/errorResult.h \
    ../../src/exceptions/exception.h \
    ../../src/exceptions/reflectionError.h \
    ../../src/exceptions/sourcelocation.h \
    ../../src/exceptions/spirvError.h \
    ../../src/extensions/deviceExtensions.h \
    ../../src/extensions/deviceLayers.h \
    ../../src/extensions/extensionRegistry.h \
    ../../src/extensions/instanceExtensions.h \
    ../../src/extensions/instanceLayers.h \
    ../../src/helpers/alignedUniformArray.h \
    ../../src/helpers/enumerationCast.h \
    ../../src/helpers/executeCommandBuffer.h \
    ../../src/helpers/executeDeferredOperation.h \
    ../../src/helpers/layoutTransition.h \
    ../../src/helpers/mapTyped.h \
    ../../src/helpers/streamInsertOperators.h \
    ../../src/helpers/stringifyFlags.h \
    ../../src/helpers/uniformArray.h \
    ../../src/misc/application.h \
    ../../src/misc/borderColor.h \
    ../../src/misc/clearAttachment.h \
    ../../src/misc/clearValue.h \
    ../../src/misc/compatibility.h \
    ../../src/misc/deviceFaultInfo.h \
    ../../src/misc/deviceFeatures.h \
    ../../src/misc/deviceQueueDescriptor.h \
    ../../src/misc/extension.h \
    ../../src/misc/featureQuery.h \
    ../../src/misc/finish.h \
    ../../src/misc/format.h \
    ../../src/misc/framebufferAttachmentImage.h \
    ../../src/misc/imageResolve.h \
    ../../src/misc/imageSubresourceRange.h \
    ../../src/misc/pipelineExecutable.h \
    ../../src/misc/pushConstantRange.h \
    ../../src/misc/scissor.h \
    ../../src/misc/scopedDebugMarker.h \
    ../../src/misc/sharing.h \
    ../../src/misc/structureChain.h \
    ../../src/misc/viewport.h \
    ../../src/objects/model/deviceChild.h \
    ../../src/objects/model/dispatchable.h \
    ../../src/objects/model/iobject.h \
    ../../src/objects/model/iresource.h \
    ../../src/objects/model/nondispatchable.h \
    ../../src/objects/model/object.h \
    ../../src/objects/model/objectType.h \
    ../../src/objects/accelerationStructure.h \
    ../../src/objects/accelerationStructureInputBuffer.h \
    ../../src/objects/accelerationStructureInstanceBuffer.h \
    ../../src/objects/baseCountBuffer.h \
    ../../src/objects/baseDeviceMemory.h \
    ../../src/objects/baseUniformBuffer.h \
    ../../src/objects/bottomLevelStructure.h \
    ../../src/objects/buffer.h \
    ../../src/objects/bufferView.h \
    ../../src/objects/commandBuffer.h \
    ../../src/objects/commandPool.h \
    ../../src/objects/computePipeline.h \
    ../../src/objects/computePipelineBatch.h \
    ../../src/objects/computePipelineLibrary.h \
    ../../src/objects/conditionalRenderingBuffer.h \
    ../../src/objects/countBuffer.h \
    ../../src/objects/debugReportCallback.h \
    ../../src/objects/debugUtilsMessenger.h \
    ../../src/objects/deferredOperation.h \
    ../../src/objects/descriptorPool.h \
    ../../src/objects/descriptorSet.h \
    ../../src/objects/descriptorSetLayout.h \
    ../../src/objects/device.h \
    ../../src/objects/deviceMemory.h \
    ../../src/objects/deviceResourcePool.h \
    ../../src/objects/display.h \
    ../../src/objects/displayMode.h \
    ../../src/objects/dstTransferBuffer.h \
    ../../src/objects/dynamicUniformBuffer.h \
    ../../src/objects/event.h \
    ../../src/objects/externalImage.h \
    ../../src/objects/fence.h \
    ../../src/objects/fragmentDensityMap.h \
    ../../src/objects/fragmentDensityMapView.h \
    ../../src/objects/fragmentShadingRateAttachment.h \
    ../../src/objects/framebuffer.h \
    ../../src/objects/fullScreenExclusiveSwapchain.h \
    ../../src/objects/graphicsPipeline.h \
    ../../src/objects/graphicsPipelineBatch.h \
    ../../src/objects/graphicsPipelineLibrary.h \
    ../../src/objects/ideviceMemory.h \
    ../../src/objects/image.h \
    ../../src/objects/image1D.h \
    ../../src/objects/image1DArray.h \
    ../../src/objects/image2D.h \
    ../../src/objects/image2DArray.h \
    ../../src/objects/image3D.h \
    ../../src/objects/imageAttachment.h \
    ../../src/objects/imageCube.h \
    ../../src/objects/imagelessFramebuffer.h \
    ../../src/objects/imageView.h \
    ../../src/objects/indexBuffer.h \
    ../../src/objects/indirectBuffer.h \
    ../../src/objects/instance.h \
    ../../src/objects/leanCommandBuffer.h \
    ../../src/objects/managedDeviceMemory.h \
    ../../src/objects/multiviewRenderPass.h \
    ../../src/objects/mutableImage.h \
    ../../src/objects/mutableImageView.h \
    ../../src/objects/physicalDevice.h \
    ../../src/objects/physicalDeviceGroup.h \
    ../../src/objects/pipeline.h \
    ../../src/objects/pipelineBatch.h \
    ../../src/objects/pipelineCache.h \
    ../../src/objects/pipelineLayout.h \
    ../../src/objects/pipelineLibrary.h \
    ../../src/objects/primaryCommandBuffer.h \
    ../../src/objects/privateDataSlot.h \
    ../../src/objects/queryPool.h \
    ../../src/objects/queue.h \
    ../../src/objects/rayTracingPipeline.h \
    ../../src/objects/rayTracingPipelineBatch.h \
    ../../src/objects/renderPass.h \
    ../../src/objects/resource.h \
    ../../src/objects/sampler.h \
    ../../src/objects/samplerYcbcrConversion.h \
    ../../src/objects/semaphore.h \
    ../../src/objects/shaderModule.h \
    ../../src/objects/srcTransferBuffer.h \
    ../../src/objects/storageBuffer.h \
    ../../src/objects/storageTexelBuffer.h \
    ../../src/objects/surface.h \
    ../../src/objects/swapchain.h \
    ../../src/objects/swapchainImage.h \
    ../../src/objects/timelineSemaphore.h \
    ../../src/objects/topLevelStructure.h \
    ../../src/objects/transformFeedbackBuffer.h \
    ../../src/objects/typedefs.h \
    ../../src/objects/uniformBuffer.h \
    ../../src/objects/uniformTexelBuffer.h \
    ../../src/objects/validationCache.h \
    ../../src/objects/vertexBuffer.h \
    ../../src/packed/color.h \
    ../../src/packed/expPow2.h \
    ../../src/packed/ieee754.h \
    ../../src/packed/rcpExpPow2.h \
    ../../src/packed/vector.h \
    ../../src/platform/androidHardwareBuffer.h \
    ../../src/platform/androidSurface.h \
    ../../src/platform/d3dExternalSemaphore.h \
    ../../src/platform/directFbSurface.h \
    ../../src/platform/externalFence.h \
    ../../src/platform/externalSemaphore.h \
    ../../src/platform/fuchsiaSurface.h \
    ../../src/platform/iosSurface.h \
    ../../src/platform/macosSurface.h \
    ../../src/platform/metalSurface.h \
    ../../src/platform/qnxSurface.h \
    ../../src/platform/typedefs.h \
    ../../src/platform/viSurface.h \
    ../../src/platform/waylandSurface.h \
    ../../src/platform/win32ExternalSemaphore.h \
    ../../src/platform/win32Surface.h \
    ../../src/platform/xcbSurface.h \
    ../../src/platform/xlibSurface.h \
    ../../src/raytracing/accelerationStructureGeometry.h \
    ../../src/raytracing/accelerationStructureHeader.h \
    ../../src/raytracing/accelerationStructureInstance.h \
    ../../src/raytracing/address.h \
    ../../src/renderpass/attachment.h \
    ../../src/renderpass/loadStoreOp.h \
    ../../src/renderpass/subpass.h \
    ../../src/shaders/pipelineShaderStage.h \
    ../../src/shaders/rayTracingShaderGroup.h \
    ../../src/shaders/shaderBindingTable.h \
    ../../src/shaders/shaderGroup.h \
    ../../src/shaders/shaderReflection.h \
    ../../src/shaders/shaderReflectionFactory.h \
    ../../src/shaders/shaderStages.h \
    ../../src/shaders/specialization.h \
    ../../src/states/colorBlendAttachmentState.h \
    ../../src/states/colorBlendState.h \
    ../../src/states/colorBlendStateAdvanced.h \
    ../../src/states/colorLogicOpState.h \
    ../../src/states/colorMask.h \
    ../../src/states/conservativeRasterizationState.h \
    ../../src/states/depthComparisonSamplerState.h \
    ../../src/states/depthStencilState.h \
    ../../src/states/formattedOutput.h \
    ../../src/states/inputAssemblyState.h \
    ../../src/states/lineRasterizationState.h \
    ../../src/states/minmaxSamplerState.h \
    ../../src/states/multiColorBlendState.h \
    ../../src/states/multisampleCoverageReductionState.h \
    ../../src/states/multisampleState.h \
    ../../src/states/multiViewportState.h \
    ../../src/states/provokingVertexState.h \
    ../../src/states/rasterizationOrderState.h \
    ../../src/states/rasterizationState.h \
    ../../src/states/samplerState.h \
    ../../src/states/samplerYcbcrConversionState.h \
    ../../src/states/streamRasterizationState.h \
    ../../src/states/tesselationState.h \
    ../../src/states/vertexAttributeType.h \
    ../../src/states/vertexFormat.h \
    ../../src/states/vertexInputState.h \
    ../../src/states/vertexInputStructure.h \
    ../../src/states/viewportState.h \
    ../../src/third-party/SPIRV-Reflect/spirv_reflect.h \
    ../../src/third-party/VulkanMemoryAllocator/include/vk_mem_alloc.h

GLSLC=$(VULKAN_SDK)/bin/glslangValidator

GLSL_DIR = $$PWD/../../src/auxiliary/spirv
SPIRV_DIR = $$GLSL_DIR/output

make_spirv_dir.target = make_spirv_dir
win32 {
    SPIRV_DIR = $$replace(SPIRV_DIR, '/', '\\')
    make_spirv_dir.commands = if not exist $$SPIRV_DIR mkdir $$SPIRV_DIR
} else {
    make_spirv_dir.commands = rm -rf $$SPIRV_DIR && mkdir $$SPIRV_DIR
}

defineReplace(compileShader) {
    GLSL_FILE = $$GLSL_DIR/$$1
    SPIRV_FILE = $$SPIRV_DIR/$$2
    !exists($$GLSL_FILE): error(Missing GLSL shader file $$GLSL_FILE)
    !exists($$SPIRV_FILE): return(@if not exist $$SPIRV_FILE $$GLSLC -V $$4 --vn $$3 $$GLSL_FILE -o $$SPIRV_FILE$$escape_expand(\\n\\t))
    return
}

compile_shaders.target = compile_shaders
compile_shaders.commands += $$compileShader(blit.vert, blitv, vsBlit, "")
compile_shaders.commands += $$compileShader(blit.vert, blitv_nv, vsBlitNV, "-DNV")
compile_shaders.commands += $$compileShader(blit.frag, blitf, fsBlit, "")
compile_shaders.commands += $$compileShader(font.frag, fontf, fsFont, "")
compile_shaders.commands += $$compileShader(imm.vert, immv, vsImm, "")
compile_shaders.commands += $$compileShader(imm.frag, immf, fsImm, "")

QMAKE_EXTRA_TARGETS = make_spirv_dir compile_shaders
PRE_TARGETDEPS = make_spirv_dir compile_shaders

INCLUDEPATH += $(VULKAN_SDK)/include
INCLUDEPATH += ../../src/third-party/pfr/include
INCLUDEPATH += ../../src/core/

QMAKE_CXXFLAGS += -std=c++17 -msse4 -ftemplate-depth=2048 -fconstexpr-depth=2048
QMAKE_CXXFLAGS += -Wno-unknown-pragmas -Wno-deprecated-copy -Wno-missing-field-initializers

win32 {
    QMAKE_CXXFLAGS += -DVK_USE_PLATFORM_WIN32_KHR
} else:macx {
    QMAKE_CXXFLAGS += -DVK_USE_PLATFORM_MACOS_MVK
} else:unix {
    QMAKE_CXXFLAGS += -DVK_USE_PLATFORM_XCB_KHR
} else:android {
    QMAKE_CXXFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
} else:ios {
    QMAKE_CXXFLAGS += -DVK_USE_PLATFORM_IOS_MVK
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
