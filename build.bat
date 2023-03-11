@echo off
rem Magma - Abstraction layer over Khronos Vulkan API.
rem Copyright (C) 2018-2023 Victor Coda.

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

set GLSL_DIR=auxiliary\spirv
set SPIRV_DIR=%GLSL_DIR%\output

set LIBRARY=magma
set PCH_FILE=%BUILD_DIR%\%LIBRARY%.pch
set PDB_FILE=%BUILD_DIR%\%LIBRARY%.pdb
set TARGET=%LIBRARY%.lib

set BASE_CFLAGS=/c /std:c++14 /EHsc /GS /Gd /permissive- /MP /nologo ^
  /W4 /WX /wd"4307" /wd"4324" /wd"4458" ^
  /constexpr:depth2048 /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /errorReport:prompt ^
  /I%VK_SDK_PATH%\Include /Icore ^
  /D %PLATFORM% /D "_UNICODE" /D "UNICODE" ^
  /Fo%BUILD_DIR%"\\" /Fd%PDB_FILE%

if %DEBUG%==1 (
  set CFLAGS=%BASE_CFLAGS% /Od /ZI /Gm- /sdl /RTC1 /MDd /D _DEBUG
) else (
  set CFLAGS=%BASE_CFLAGS% /O2 /Oi /GL /Zi /Gm- /Gy /MT /D NDEBUG
)

set BASE_LDFLAGS=/machine:X64 /libpath:%BUILD_DIR% /out:%BUILD_DIR%\%TARGET% /nologo

if %DEBUG%==1 (
  set LDFLAGS=%BASE_LDFLAGS%
) else (
  set LDFLAGS=%BASE_LDFLAGS% /LTCG
)

set glslc=%VK_SDK_PATH%\Bin\glslangValidator

set MAGMA_SRC= ^
allocator\alignedAllocator.cpp ^
allocator\allocationCallbacks.cpp ^
allocator\cxxAllocator.cpp ^
allocator\deviceMemoryAllocator.cpp ^
allocator\vmaImplementation.cpp ^
auxiliary\accumulationBuffer.cpp ^
auxiliary\baseFramebuffer.cpp ^
auxiliary\blitRectangle.cpp ^
auxiliary\colorFramebuffer.cpp ^
auxiliary\colorMultisampleFramebuffer.cpp ^
auxiliary\depthFramebuffer.cpp ^
auxiliary\fillRectangleVertexShader.cpp ^
auxiliary\frameGrabber.cpp ^
auxiliary\generateMipmap.cpp ^
auxiliary\graphicsPipelineCache.cpp ^
auxiliary\imageDescriptorSet.cpp ^
auxiliary\immediateRender.cpp ^
auxiliary\multiAttachmentFramebuffer.cpp ^
auxiliary\profiler.cpp ^
auxiliary\sprite.cpp ^
auxiliary\swapchainFramebuffer.cpp ^
auxiliary\textShader.cpp ^
barriers\bufferMemoryBarrier.cpp ^
barriers\imageMemoryBarrier.cpp ^
descriptors\accelerationStructureDescriptor.cpp ^
descriptors\descriptor.cpp ^
descriptors\subpass.cpp ^
exceptions\exception.cpp ^
extensions\extensions.cpp ^
extensions\instanceExtensions.cpp ^
extensions\instanceLayers.cpp ^
extensions\layers.cpp ^
extensions\physicalDeviceExtensions.cpp ^
helpers\checkFeatureSupport.cpp ^
helpers\executeCommandBuffer.cpp ^
helpers\executeDeferredOperation.cpp ^
helpers\stringize.cpp ^
misc\deviceFaultInfo.cpp ^
misc\deviceFeatures.cpp ^
misc\deviceQueueDescriptor.cpp ^
misc\format.cpp ^
misc\framebufferAttachmentImage.cpp ^
misc\geometry.cpp ^
misc\pipelineExecutable.cpp ^
misc\scopedDebugMarker.cpp ^
misc\sharing.cpp ^
misc\structureChain.cpp ^
objects\accelerationStructure.cpp ^
objects\accelerationStructureInstanceBuffer.cpp ^
objects\buffer.cpp ^
objects\bufferView.cpp ^
objects\commandBuffer.cpp ^
objects\commandBufferDebug.cpp ^
objects\commandPool.cpp ^
objects\computePipeline.cpp ^
objects\computePipelines.cpp ^
objects\conditionalRenderingBuffer.cpp ^
objects\debugReportCallback.cpp ^
objects\debugUtilsMessenger.cpp ^
objects\deferredOperation.cpp ^
objects\descriptorPool.cpp ^
objects\descriptorSet.cpp ^
objects\descriptorSetLayout.cpp ^
objects\device.cpp ^
objects\deviceMemory.cpp ^
objects\display.cpp ^
objects\displayMode.cpp ^
objects\dstTransferBuffer.cpp ^
objects\event.cpp ^
objects\fence.cpp ^
objects\fragmentDensityMap.cpp ^
objects\fragmentDensityMapView.cpp ^
objects\fragmentShadingRateAttachment.cpp ^
objects\framebuffer.cpp ^
objects\fullScreenExclusiveSwapchain.cpp ^
objects\graphicsPipeline.cpp ^
objects\graphicsPipelines.cpp ^
objects\image.cpp ^
objects\image1D.cpp ^
objects\image1DArray.cpp ^
objects\image2D.cpp ^
objects\image2DArray.cpp ^
objects\image3D.cpp ^
objects\imageAttachment.cpp ^
objects\imageCube.cpp ^
objects\imagelessFramebuffer.cpp ^
objects\imageView.cpp ^
objects\indexBuffer.cpp ^
objects\indirectBuffer.cpp ^
objects\instance.cpp ^
objects\mutableImage.cpp ^
objects\mutableImageView.cpp ^
objects\object.cpp ^
objects\physicalDevice.cpp ^
objects\physicalDeviceGroup.cpp ^
objects\pipeline.cpp ^
objects\pipelineCache.cpp ^
objects\pipelineLayout.cpp ^
objects\primaryCommandBuffer.cpp ^
objects\queryPool.cpp ^
objects\queue.cpp ^
objects\rayTracingBuffer.cpp ^
objects\rayTracingPipeline.cpp ^
objects\rayTracingPipelines.cpp ^
objects\renderPass.cpp ^
objects\renderPassMultiview.cpp ^
objects\resource.cpp ^
objects\resourcePool.cpp ^
objects\sampler.cpp ^
objects\samplerYcbcrConversion.cpp ^
objects\semaphore.cpp ^
objects\shaderModule.cpp ^
objects\srcTransferBuffer.cpp ^
objects\storageBuffer.cpp ^
objects\storageTexelBuffer.cpp ^
objects\surface.cpp ^
objects\swapchain.cpp ^
objects\swapchainImage.cpp ^
objects\timelineSemaphore.cpp ^
objects\transformFeedbackBuffer.cpp ^
objects\uniformTexelBuffer.cpp ^
objects\validationCache.cpp ^
objects\vertexBuffer.cpp ^
shaders\pipelineShaderStage.cpp ^
shaders\rayTracingShaderGroup.cpp ^
shaders\shaderBindingTable.cpp ^
shaders\shaderReflection.cpp ^
shaders\specialization.cpp ^
states\colorBlendState.cpp ^
states\colorBlendStateAdvanced.cpp ^
states\multisampleState.cpp ^
states\multiViewportState.cpp ^
states\rasterizationState.cpp ^
states\vertexInputState.cpp ^
states\vertexInputStructure.cpp ^
states\viewportState.cpp

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
cl %CFLAGS% /Yc"pch.h" /Fp%PCH_FILE% core\pch.cpp

echo Compiling library
cl %CFLAGS% /Yu"pch.h" /Fp%PCH_FILE% %MAGMA_SRC%

rem Third party source files
cl %CFLAGS% third-party\SPIRV-Reflect\spirv_reflect.c

set MAGMA_OBJS= ^
alignedAllocator.obj ^
allocationCallbacks.obj ^
cxxAllocator.obj ^
deviceMemoryAllocator.obj ^
vmaImplementation.obj ^
accumulationBuffer.obj ^
baseFramebuffer.obj ^
blitRectangle.obj ^
colorFramebuffer.obj ^
colorMultisampleFramebuffer.obj ^
depthFramebuffer.obj ^
fillRectangleVertexShader.obj ^
frameGrabber.obj ^
generateMipmap.obj ^
graphicsPipelineCache.obj ^
imageDescriptorSet.obj ^
immediateRender.obj ^
multiAttachmentFramebuffer.obj ^
profiler.obj ^
sprite.obj ^
swapchainFramebuffer.obj ^
textShader.obj ^
bufferMemoryBarrier.obj ^
imageMemoryBarrier.obj ^
pch.obj ^
accelerationStructureDescriptor.obj ^
descriptor.obj ^
subpass.obj ^
exception.obj ^
extensions.obj ^
instanceExtensions.obj ^
instanceLayers.obj ^
layers.obj ^
physicalDeviceExtensions.obj ^
checkFeatureSupport.obj ^
executeCommandBuffer.obj ^
executeDeferredOperation.obj ^
stringize.obj ^
deviceFaultInfo.obj ^
deviceFeatures.obj ^
deviceQueueDescriptor.obj ^
format.obj ^
framebufferAttachmentImage.obj ^
geometry.obj ^
pipelineExecutable.obj ^
scopedDebugMarker.obj ^
sharing.obj ^
structureChain.obj ^
accelerationStructure.obj ^
accelerationStructureInstanceBuffer.obj ^
buffer.obj ^
bufferView.obj ^
commandBuffer.obj ^
commandBufferDebug.obj ^
commandPool.obj ^
computePipeline.obj ^
computePipelines.obj ^
conditionalRenderingBuffer.obj ^
debugReportCallback.obj ^
debugUtilsMessenger.obj ^
deferredOperation.obj ^
descriptorPool.obj ^
descriptorSet.obj ^
descriptorSetLayout.obj ^
device.obj ^
deviceMemory.obj ^
display.obj ^
displayMode.obj ^
dstTransferBuffer.obj ^
event.obj ^
fence.obj ^
fragmentDensityMap.obj ^
fragmentDensityMapView.obj ^
fragmentShadingRateAttachment.obj ^
framebuffer.obj ^
fullScreenExclusiveSwapchain.obj ^
graphicsPipeline.obj ^
graphicsPipelines.obj ^
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
mutableImage.obj ^
mutableImageView.obj ^
object.obj ^
physicalDevice.obj ^
physicalDeviceGroup.obj ^
pipeline.obj ^
pipelineCache.obj ^
pipelineLayout.obj ^
primaryCommandBuffer.obj ^
queryPool.obj ^
queue.obj ^
rayTracingBuffer.obj ^
rayTracingPipeline.obj ^
rayTracingPipelines.obj ^
renderPass.obj ^
renderPassMultiview.obj ^
resource.obj ^
resourcePool.obj ^
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
transformFeedbackBuffer.obj ^
uniformTexelBuffer.obj ^
validationCache.obj ^
vertexBuffer.obj ^
pipelineShaderStage.obj ^
rayTracingShaderGroup.obj ^
shaderBindingTable.obj ^
shaderReflection.obj ^
specialization.obj ^
colorBlendState.obj ^
colorBlendStateAdvanced.obj ^
multisampleState.obj ^
multiViewportState.obj ^
rasterizationState.obj ^
vertexInputState.obj ^
vertexInputStructure.obj ^
viewportState.obj ^
spirv_reflect.obj

echo Linking library %BUILD_DIR%\%TARGET%
lib %LDFLAGS% %MAGMA_OBJS%
