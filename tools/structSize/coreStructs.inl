source << "case VK_STRUCTURE_TYPE_APPLICATION_INFO: return sizeof(VkApplicationInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO: return sizeof(VkInstanceCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO: return sizeof(VkDeviceQueueCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO: return sizeof(VkDeviceCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_SUBMIT_INFO: return sizeof(VkSubmitInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO: return sizeof(VkMemoryAllocateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE: return sizeof(VkMappedMemoryRange);" << std::endl;
source << "case VK_STRUCTURE_TYPE_BIND_SPARSE_INFO: return sizeof(VkBindSparseInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_FENCE_CREATE_INFO: return sizeof(VkFenceCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO: return sizeof(VkSemaphoreCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_EVENT_CREATE_INFO: return sizeof(VkEventCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO: return sizeof(VkQueryPoolCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO: return sizeof(VkBufferCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO: return sizeof(VkBufferViewCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO: return sizeof(VkImageCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO: return sizeof(VkImageViewCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO: return sizeof(VkShaderModuleCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO: return sizeof(VkPipelineCacheCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO: return sizeof(VkPipelineShaderStageCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO: return sizeof(VkPipelineVertexInputStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO: return sizeof(VkPipelineInputAssemblyStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO: return sizeof(VkPipelineTessellationStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO: return sizeof(VkPipelineViewportStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO: return sizeof(VkPipelineRasterizationStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO: return sizeof(VkPipelineMultisampleStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO: return sizeof(VkPipelineDepthStencilStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO: return sizeof(VkPipelineColorBlendStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO: return sizeof(VkPipelineDynamicStateCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO: return sizeof(VkGraphicsPipelineCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO: return sizeof(VkComputePipelineCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO: return sizeof(VkPipelineLayoutCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO: return sizeof(VkSamplerCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO: return sizeof(VkDescriptorSetLayoutCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO: return sizeof(VkDescriptorPoolCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO: return sizeof(VkDescriptorSetAllocateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET: return sizeof(VkWriteDescriptorSet);" << std::endl;
source << "case VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET: return sizeof(VkCopyDescriptorSet);" << std::endl;
source << "case VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO: return sizeof(VkFramebufferCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO: return sizeof(VkRenderPassCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO: return sizeof(VkCommandPoolCreateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO: return sizeof(VkCommandBufferAllocateInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO: return sizeof(VkCommandBufferInheritanceInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO: return sizeof(VkCommandBufferBeginInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO: return sizeof(VkRenderPassBeginInfo);" << std::endl;
source << "case VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER: return sizeof(VkBufferMemoryBarrier);" << std::endl;
source << "case VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER: return sizeof(VkImageMemoryBarrier);" << std::endl;
source << "case VK_STRUCTURE_TYPE_MEMORY_BARRIER: return sizeof(VkMemoryBarrier);" << std::endl;
// Reserved for internal use by the loader, and do not have corresponding Vulkan structures
source << "case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: return 0;" << std::endl;
source << "case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: return 0;" << std::endl;
