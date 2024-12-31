/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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

namespace magma
{
    /* Load/store ops specifies how contents of an attachment
       are treated at the beginning and at the end of a subpass. */

    struct LoadStoreOp
    {
        VkAttachmentLoadOp load;
        VkAttachmentStoreOp store;
    };

    namespace op
    {
        constexpr LoadStoreOp load{VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_DONT_CARE};
        constexpr LoadStoreOp clear{VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE};
        constexpr LoadStoreOp store{VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE};

        constexpr LoadStoreOp loadStore{VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE};
        constexpr LoadStoreOp clearStore{VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE};
        constexpr LoadStoreOp dontCare{VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE};

        /* Renderpass attachments can be read-only for the duration
           of a render pass. Examples include input attachments and
           depth attachments where depth tests are enabled but depth
           writes are not enabled. In such cases, there can be no
           contents generated for an attachment within the render area. */

    #ifdef VK_QCOM_render_pass_store_ops
        constexpr LoadStoreOp loadNone{VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_NONE_QCOM};
        constexpr LoadStoreOp clearNone{VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_NONE_QCOM};
        constexpr LoadStoreOp dontCareNone{VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_NONE_QCOM};
    #endif // VK_QCOM_render_pass_store_ops

        /* Useful as an alternative to preserve attachments in applications
           that cannot decide if an attachment will be used in a render
           pass until after the necessary pipelines have been created. */

    #ifdef VK_EXT_load_store_op_none
        constexpr LoadStoreOp none{VK_ATTACHMENT_LOAD_OP_NONE_EXT, VK_ATTACHMENT_STORE_OP_NONE_EXT};
    #endif // VK_EXT_load_store_op_none
    } // namespace op
} // namespace magma
