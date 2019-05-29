/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../api/vulkan.h"

namespace magma
{
    class LoadStoreOp
    {
    public:
        constexpr LoadStoreOp(VkAttachmentLoadOp loadOp,
            VkAttachmentStoreOp storeOp) noexcept:
            loadOp(loadOp), storeOp(storeOp) {}

    private:
        VkAttachmentLoadOp loadOp;
        VkAttachmentStoreOp storeOp;
        friend struct AttachmentDescription;
    };

    /* An attachment description describes the properties of an attachment
       including its format, sample count, and how its contents are treated
       at the beginning and end of each render pass instance. */

    struct AttachmentDescription : VkAttachmentDescription
    {
        constexpr AttachmentDescription(VkFormat format,
            uint32_t sampleCount,
            VkAttachmentLoadOp loadOp,
            VkAttachmentStoreOp storeOp,
            VkAttachmentLoadOp stencilLoadOp,
            VkAttachmentStoreOp stencilStoreOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout) noexcept;
        constexpr AttachmentDescription(VkFormat format,
            uint32_t sampleCount,
            const LoadStoreOp& colorDepthOp,
            const LoadStoreOp& stencilOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout) noexcept;
        constexpr AttachmentDescription(const LoadStoreOp& colorDepthOp,
            const LoadStoreOp& stencilOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout) noexcept;
        constexpr AttachmentDescription(VkFormat format,
            uint32_t sampleCount,
            const AttachmentDescription& prefab) noexcept;
    };
} // namespace magma

#include "attachment.inl"

namespace magma
{
    namespace op
    {
        constexpr LoadStoreOp loadStore(VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE);
        constexpr LoadStoreOp clearStore(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE);
        constexpr LoadStoreOp dontCareStore(VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE);
        constexpr LoadStoreOp loadDontCare(VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_DONT_CARE);
        constexpr LoadStoreOp clearDontCare(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE);
        constexpr LoadStoreOp dontCare(VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE);
    } // namespace op

    namespace attachments
    {
        constexpr AttachmentDescription colorLoadStoreAttachment(op::loadStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription colorClearStoreAttachment(op::clearStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription colorDontCareStoreAttachment(op::dontCareStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription colorLoadDontCareAttachment(op::loadDontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription colorClearDontCareAttachment(op::clearDontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription colorDontCareAttachment(op::dontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        constexpr AttachmentDescription colorLoadStoreShaderReadOnly(op::loadStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription colorClearStoreShaderReadOnly(op::clearStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription colorDontCareStoreShaderReadOnly(op::dontCareStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription colorLoadDontCareShaderReadOnly(op::loadDontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription colorClearDontCareShaderReadOnly(op::clearDontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription colorDontCareShaderReadOnly(op::dontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        constexpr AttachmentDescription depthLoadStoreAttachment(op::loadStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription depthClearStoreAttachment(op::clearStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription depthDontCareStoreAttachment(op::dontCareStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription depthLoadDontCareAttachment(op::loadDontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription depthClearDontCareAttachment(op::clearDontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        
        constexpr AttachmentDescription depthLoadStoreShaderReadOnly(op::loadStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription depthClearStoreShaderReadOnly(op::clearStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription depthDontCareStoreShaderReadOnly(op::dontCareStore, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription depthLoadDontCareShaderReadOnly(op::loadDontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription depthClearDontCareShaderReadOnly(op::clearDontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

        constexpr AttachmentDescription stencilLoadStoreAttachment(op::dontCare, op::loadStore, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription stencilClearStoreAttachment(op::dontCare, op::clearStore, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription stencilDontCareStoreAttachment(op::dontCare, op::dontCareStore, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription stencilLoadDontCareAttachment(op::dontCare, op::loadDontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription stencilClearDontCareAttachment(op::dontCare, op::clearDontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        
        constexpr AttachmentDescription stencilLoadStoreShaderReadOnly(op::dontCare, op::loadStore, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription stencilClearStoreShaderReadOnly(op::dontCare, op::clearStore, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription stencilDontCareStoreShaderReadOnly(op::dontCare, op::dontCareStore, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription stencilLoadDontCareShaderReadOnly(op::dontCare, op::loadDontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        constexpr AttachmentDescription stencilClearDontCareShaderReadOnly(op::dontCare, op::clearDontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

        constexpr AttachmentDescription depthStencilDontCareAttachment(op::dontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        constexpr AttachmentDescription depthStencilDontCareShaderReadOnly(op::dontCare, op::dontCare, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
    } // namespace attachments
} // namespace magma
