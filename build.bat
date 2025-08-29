@echo off
rem Magma - Abstraction layer over Khronos Vulkan API.
rem Copyright (C) 2018-2024 Victor Coda.

rem This program is free software: you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation, either version 3 of the License, or
rem (at your option) any later version.

rem This program is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
rem GNU General Public License for more details.

rem You should have received a copy of the GNU General Public License
rem along with this program. If not, see <https://www.gnu.org/licenses/>.

if not defined DEBUG (
  set DEBUG=1
)

set PLATFORM=VK_USE_PLATFORM_WIN32_KHR
set ARCH=x64

set PROJECT_DIR=projects\vs

if %DEBUG%==1 (
  set BUILD_DIR=%PROJECT_DIR%\%ARCH%\Debug
) else (
  set BUILD_DIR=%PROJECT_DIR%\%ARCH%\Release
)

set GLSL_DIR=src\auxiliary\spirv
set SPIRV_DIR=%GLSL_DIR%\output

set LIBRARY=magma
set PCH_FILE=%BUILD_DIR%\%LIBRARY%.pch
set PDB_FILE=%BUILD_DIR%\%LIBRARY%.pdb
set TARGET=%LIBRARY%.lib

set BASE_CFLAGS=/c /std:c++17 /EHsc /GS /Gd /permissive- /MP /nologo ^
  /W4 /WX /wd"4065" /wd"4348" /wd"4458" ^
  /constexpr:depth2048 /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /errorReport:prompt ^
  /I%VULKAN_SDK%\Include /Icore /Isrc\third-party\pfr\include ^
  /D %PLATFORM% /D "_UNICODE" /D "UNICODE" ^
  /Fo%BUILD_DIR%"\\" /Fd%PDB_FILE%

if %DEBUG%==1 (
  set CFLAGS=%BASE_CFLAGS% /Od /ZI /Gm- /sdl /RTC1 /MDd /D _DEBUG
) else (
  set CFLAGS=%BASE_CFLAGS% /O2 /Oi /GL /Zi /Gm- /Gy /MT /D NDEBUG
)

set BASE_LDFLAGS=/machine:X64 /libpath:%BUILD_DIR% /out:%BUILD_DIR%\%TARGET% /nologo /ignore:4221

if %DEBUG%==1 (
  set LDFLAGS=%BASE_LDFLAGS%
) else (
  set LDFLAGS=%BASE_LDFLAGS% /LTCG
)

set glslc=%VULKAN_SDK%\Bin\glslangValidator

set MAGMA_SRC= ^
src\allocator\alignedAllocator.cpp ^
src\allocator\allocationCallbacks.cpp ^
src\allocator\debugAlignedAllocator.cpp ^
src\allocator\deviceMemoryAllocator.cpp ^
src\auxiliary\accumulationBuffer.cpp ^
src\auxiliary\auxframebuffer.cpp ^
src\auxiliary\blitRectangle.cpp ^
src\auxiliary\fillRectangleVertexShader.cpp ^
src\auxiliary\frameGrabber.cpp ^
src\auxiliary\generateMipmap.cpp ^
src\auxiliary\graphicsPipelineCache.cpp ^
src\auxiliary\imageDescriptorSet.cpp ^
src\auxiliary\immediateRender.cpp ^
src\auxiliary\profiler.cpp ^
src\auxiliary\shaderCompiler.cpp ^
src\auxiliary\sprite.cpp ^
src\auxiliary\textShader.cpp ^
src\barriers\imageMemoryBarrier.cpp ^
src\descriptors\accelerationStructureDescriptor.cpp ^
src\descriptors\bufferDescriptor.cpp ^
src\descriptors\imageDescriptor.cpp ^
src\exceptions\errorHandler.cpp ^
src\exceptions\errorResult.cpp ^
src\exceptions\reflectionError.cpp ^
src\exceptions\spirvError.cpp ^
src\extensions\deviceExtensions.cpp ^
src\extensions\deviceLayers.cpp ^
src\extensions\instanceExtensions.cpp ^
src\extensions\instanceLayers.cpp ^
src\helpers\executeCommandBuffer.cpp ^
src\helpers\executeDeferredOperation.cpp ^
src\helpers\layoutTransition.cpp ^
src\helpers\streamInsertOperators.cpp ^
src\helpers\stringifyFlags.cpp ^
src\misc\application.cpp ^
src\misc\deviceFaultInfo.cpp ^
src\misc\deviceFeatures.cpp ^
src\misc\deviceQueueDescriptor.cpp ^
src\misc\featureQuery.cpp ^
src\misc\finish.cpp ^
src\misc\format.cpp ^
src\misc\framebufferAttachmentImage.cpp ^
src\misc\pipelineExecutable.cpp ^
src\misc\scopedDebugMarker.cpp ^
src\misc\sharing.cpp ^
src\misc\structureChain.cpp ^
src\objects\model\deviceChild.cpp ^
src\objects\accelerationStructure.cpp ^
src\objects\accelerationStructureInputBuffer.cpp ^
src\objects\baseDeviceMemory.cpp ^
src\objects\baseUniformBuffer.cpp ^
src\objects\bottomLevelStructure.cpp ^
src\objects\buffer.cpp ^
src\objects\bufferView.cpp ^
src\objects\commandBuffer.cpp ^
src\objects\commandBufferDebug.cpp ^
src\objects\commandPool.cpp ^
src\objects\computePipeline.cpp ^
src\objects\computePipelineBatch.cpp ^
src\objects\computePipelineLibrary.cpp ^
src\objects\conditionalRenderingBuffer.cpp ^
src\objects\countBuffer.cpp ^
src\objects\debugReportCallback.cpp ^
src\objects\debugUtilsMessenger.cpp ^
src\objects\deferredOperation.cpp ^
src\objects\descriptorPool.cpp ^
src\objects\descriptorSet.cpp ^
src\objects\descriptorSetLayout.cpp ^
src\objects\device.cpp ^
src\objects\deviceMemory.cpp ^
src\objects\deviceResourcePool.cpp ^
src\objects\display.cpp ^
src\objects\displayMode.cpp ^
src\objects\dstTransferBuffer.cpp ^
src\objects\dynamicUniformBuffer.cpp ^
src\objects\event.cpp ^
src\objects\externalImage.cpp ^
src\objects\fence.cpp ^
src\objects\fragmentDensityMap.cpp ^
src\objects\fragmentDensityMapView.cpp ^
src\objects\fragmentShadingRateAttachment.cpp ^
src\objects\framebuffer.cpp ^
src\objects\fullScreenExclusiveSwapchain.cpp ^
src\objects\graphicsPipeline.cpp ^
src\objects\graphicsPipelineBatch.cpp ^
src\objects\graphicsPipelineLibrary.cpp ^
src\objects\image.cpp ^
src\objects\image1D.cpp ^
src\objects\image1DArray.cpp ^
src\objects\image2D.cpp ^
src\objects\image2DArray.cpp ^
src\objects\image3D.cpp ^
src\objects\imageAttachment.cpp ^
src\objects\imageCube.cpp ^
src\objects\imagelessFramebuffer.cpp ^
src\objects\imageView.cpp ^
src\objects\indexBuffer.cpp ^
src\objects\indirectBuffer.cpp ^
src\objects\instance.cpp ^
src\objects\leanCommandBuffer.cpp ^
src\objects\managedDeviceMemory.cpp ^
src\objects\multiviewRenderPass.cpp ^
src\objects\mutableImage.cpp ^
src\objects\mutableImageView.cpp ^
src\objects\physicalDevice.cpp ^
src\objects\physicalDeviceFeatures.cpp ^
src\objects\physicalDeviceGroup.cpp ^
src\objects\pipeline.cpp ^
src\objects\pipelineBatch.cpp ^
src\objects\pipelineCache.cpp ^
src\objects\pipelineLayout.cpp ^
src\objects\pipelineLibrary.cpp ^
src\objects\primaryCommandBuffer.cpp ^
src\objects\privateDataSlot.cpp ^
src\objects\queryPool.cpp ^
src\objects\queue.cpp ^
src\objects\rayTracingPipeline.cpp ^
src\objects\rayTracingPipelineBatch.cpp ^
src\objects\renderPass.cpp ^
src\objects\resource.cpp ^
src\objects\sampler.cpp ^
src\objects\samplerYcbcrConversion.cpp ^
src\objects\semaphore.cpp ^
src\objects\shaderModule.cpp ^
src\objects\srcTransferBuffer.cpp ^
src\objects\storageBuffer.cpp ^
src\objects\storageTexelBuffer.cpp ^
src\objects\surface.cpp ^
src\objects\swapchain.cpp ^
src\objects\swapchainImage.cpp ^
src\objects\timelineSemaphore.cpp ^
src\objects\topLevelStructure.cpp ^
src\objects\transformFeedbackBuffer.cpp ^
src\objects\uniformTexelBuffer.cpp ^
src\objects\validationCache.cpp ^
src\objects\variableCountDescriptorSet.cpp ^
src\objects\vertexBuffer.cpp ^
src\platform\d3dExternalSemaphore.cpp ^
src\platform\externalFence.cpp ^
src\platform\externalSemaphore.cpp ^
src\platform\win32ExternalSemaphore.cpp ^
src\platform\win32Surface.cpp ^
src\raytracing\accelerationStructureGeometry.cpp ^
src\renderpass\subpass.cpp ^
src\shaders\pipelineShaderStage.cpp ^
src\shaders\shaderBindingTable.cpp ^
src\shaders\shaderGroup.cpp ^
src\shaders\shaderReflection.cpp ^
src\shaders\specialization.cpp ^
src\states\colorBlendStateAdvanced.cpp ^
src\states\formattedOutput.cpp ^
src\states\multiColorBlendState.cpp ^
src\states\multisampleState.cpp ^
src\states\multiViewportState.cpp ^
src\states\rasterizationState.cpp ^
src\states\vertexInputState.cpp ^
src\states\vertexInputStructure.cpp ^
src\states\viewportState.cpp

echo Compiling shaders
%glslc% -V --vn fsBlit %GLSL_DIR%\blit.frag -o %SPIRV_DIR%\blitf
%glslc% -V --vn vsBlit %GLSL_DIR%\blit.vert -o %SPIRV_DIR%\blitv
%glslc% -V -DNV --vn vsBlitNV %GLSL_DIR%\blit.vert -o %SPIRV_DIR%\blitv_nv
%glslc% -V --vn fsFont %GLSL_DIR%\font.frag -o %SPIRV_DIR%\fontf
%glslc% -V --vn vsImm %GLSL_DIR%\imm.vert -o %SPIRV_DIR%\immv
%glslc% -V --vn fsImm %GLSL_DIR%\imm.frag -o %SPIRV_DIR%\immf

if not exist %PROJECT_DIR%\%ARCH% mkdir %PROJECT_DIR%\%ARCH%
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

echo Creating precompiled header
cl %CFLAGS% /Yc"pch.h" /Fp%PCH_FILE% src\core\pch.cpp

echo Compiling library
cl %CFLAGS% /Yu"pch.h" /Fp%PCH_FILE% %MAGMA_SRC%

rem Third party source files
cl %CFLAGS% src\third-party\SPIRV-Reflect\spirv_reflect.c

set MAGMA_OBJS= ^
alignedAllocator.obj ^
allocationCallbacks.obj ^
debugAlignedAllocator.obj ^
deviceMemoryAllocator.obj ^
accumulationBuffer.obj ^
auxframebuffer.obj ^
blitRectangle.obj ^
fillRectangleVertexShader.obj ^
frameGrabber.obj ^
generateMipmap.obj ^
graphicsPipelineCache.obj ^
imageDescriptorSet.obj ^
immediateRender.obj ^
profiler.obj ^
shaderCompiler.obj ^
sprite.obj ^
textShader.obj ^
imageMemoryBarrier.obj ^
pch.obj ^
accelerationStructureDescriptor.obj ^
bufferDescriptor.obj ^
imageDescriptor.obj ^
errorHandler.obj ^
errorResult.obj ^
reflectionError.obj ^
spirvError.obj ^
deviceExtensions.obj ^
deviceLayers.obj ^
instanceExtensions.obj ^
instanceLayers.obj ^
executeCommandBuffer.obj ^
executeDeferredOperation.obj ^
layoutTransition.obj ^
streamInsertOperators.obj ^
stringifyFlags.obj ^
application.obj ^
deviceFaultInfo.obj ^
deviceFeatures.obj ^
deviceQueueDescriptor.obj ^
featureQuery.obj ^
finish.obj ^
format.obj ^
framebufferAttachmentImage.obj ^
pipelineExecutable.obj ^
scopedDebugMarker.obj ^
sharing.obj ^
structureChain.obj ^
deviceChild.obj ^
accelerationStructure.obj ^
accelerationStructureInputBuffer.obj ^
baseDeviceMemory.obj ^
baseUniformBuffer.obj ^
bottomLevelStructure.obj ^
buffer.obj ^
bufferView.obj ^
commandBuffer.obj ^
commandBufferDebug.obj ^
commandPool.obj ^
computePipeline.obj ^
computePipelineBatch.obj ^
computePipelineLibrary.obj ^
conditionalRenderingBuffer.obj ^
countBuffer.obj ^
debugReportCallback.obj ^
debugUtilsMessenger.obj ^
deferredOperation.obj ^
descriptorPool.obj ^
descriptorSet.obj ^
descriptorSetLayout.obj ^
device.obj ^
deviceMemory.obj ^
deviceResourcePool.obj ^
display.obj ^
displayMode.obj ^
dstTransferBuffer.obj ^
dynamicUniformBuffer.obj ^
event.obj ^
externalImage.obj ^
fence.obj ^
fragmentDensityMap.obj ^
fragmentDensityMapView.obj ^
fragmentShadingRateAttachment.obj ^
framebuffer.obj ^
fullScreenExclusiveSwapchain.obj ^
graphicsPipeline.obj ^
graphicsPipelineBatch.obj ^
graphicsPipelineLibrary.obj ^
image.obj ^
image1D.obj ^
image1DArray.obj ^
image2D.obj ^
image2DArray.obj ^
image3D.obj ^
imageAttachment.obj ^
imageCube.obj ^
imagelessFramebuffer.obj ^
imageView.obj ^
indexBuffer.obj ^
indirectBuffer.obj ^
instance.obj ^
leanCommandBuffer.obj ^
managedDeviceMemory.obj ^
multiviewRenderPass.obj ^
mutableImage.obj ^
mutableImageView.obj ^
physicalDevice.obj ^
physicalDeviceFeatures.obj ^
physicalDeviceGroup.obj ^
pipeline.obj ^
pipelineBatch.obj ^
pipelineCache.obj ^
pipelineLayout.obj ^
pipelineLibrary.obj ^
primaryCommandBuffer.obj ^
privateDataSlot.obj ^
queryPool.obj ^
queue.obj ^
rayTracingPipeline.obj ^
rayTracingPipelineBatch.obj ^
renderPass.obj ^
resource.obj ^
sampler.obj ^
samplerYcbcrConversion.obj ^
semaphore.obj ^
shaderModule.obj ^
srcTransferBuffer.obj ^
storageBuffer.obj ^
storageTexelBuffer.obj ^
surface.obj ^
swapchain.obj ^
swapchainImage.obj ^
timelineSemaphore.obj ^
topLevelStructure.obj ^
transformFeedbackBuffer.obj ^
uniformTexelBuffer.obj ^
validationCache.obj ^
variableCountDescriptorSet.obj ^
vertexBuffer.obj ^
d3dExternalSemaphore.obj ^
externalFence.obj ^
externalSemaphore.obj ^
win32ExternalSemaphore.obj ^
win32Surface.obj ^
accelerationStructureGeometry.obj ^
subpass.obj ^
pipelineShaderStage.obj ^
shaderBindingTable.obj ^
shaderGroup.obj ^
shaderReflection.obj ^
specialization.obj ^
colorBlendStateAdvanced.obj ^
formattedOutput.obj ^
multiColorBlendState.obj ^
multisampleState.obj ^
multiViewportState.obj ^
rasterizationState.obj ^
vertexInputState.obj ^
vertexInputStructure.obj ^
viewportState.obj ^
spirv_reflect.obj

echo Linking library %BUILD_DIR%\%TARGET%
lib %LDFLAGS% %MAGMA_OBJS%
