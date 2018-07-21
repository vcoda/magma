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
#include "../vulkan.h"

namespace magma
{
    class LoadStoreOp
    {
    public:
        LoadStoreOp(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp) noexcept:
            loadOp(loadOp), storeOp(storeOp) {}

    private:
        VkAttachmentLoadOp loadOp;
        VkAttachmentStoreOp storeOp;
        friend struct AttachmentDescription;
    };

    struct AttachmentDescription : VkAttachmentDescription
    {
        AttachmentDescription(VkFormat format,
            uint32_t sampleCount,
            const LoadStoreOp& op,
            const LoadStoreOp& stencilOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout);
        AttachmentDescription(VkAttachmentLoadOp loadOp,
            VkAttachmentStoreOp storeOp,
            VkAttachmentLoadOp stencilLoadOp,
            VkAttachmentStoreOp stencilStoreOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout) noexcept;
        AttachmentDescription(VkFormat format, uint32_t sampleCount, const AttachmentDescription& predefined);
    };

    namespace op
    {
        extern const LoadStoreOp loadStore;
        extern const LoadStoreOp clearStore;
        extern const LoadStoreOp dontCareStore;
        extern const LoadStoreOp loadDontCare;
        extern const LoadStoreOp clearDontCare;
        extern const LoadStoreOp dontCareDontCare;
    } // namespace op

    namespace attachments
    {
        extern const AttachmentDescription colorLoadStoreOptimal;
        extern const AttachmentDescription colorClearStoreOptimal;
        extern const AttachmentDescription colorDontCareStoreOptimal;
        extern const AttachmentDescription colorLoadDontCareOptimal;
        extern const AttachmentDescription colorClearDontCareOptimal;
        extern const AttachmentDescription colorDontCareDontCareOptimal;

        extern const AttachmentDescription colorLoadStoreReadOnly;
        extern const AttachmentDescription colorClearStoreReadOnly;
        extern const AttachmentDescription colorDontCareStoreReadOnly;
        extern const AttachmentDescription colorLoadDontCareReadOnly;
        extern const AttachmentDescription colorClearDontCareReadOnly;
        extern const AttachmentDescription colorDontCareDontCareReadOnly;

        extern const AttachmentDescription depthLoadStoreOptimal;
        extern const AttachmentDescription depthClearStoreOptimal;
        extern const AttachmentDescription depthDontCareStoreOptimal;
        extern const AttachmentDescription depthLoadDontCareOptimal;
        extern const AttachmentDescription depthClearDontCareOptimal;
        extern const AttachmentDescription depthDontCareDontCareOptimal;

        extern const AttachmentDescription depthLoadStoreReadOnly;
        extern const AttachmentDescription depthClearStoreReadOnly;
        extern const AttachmentDescription depthDontCareStoreReadOnly;
        extern const AttachmentDescription depthLoadDontCareReadOnly;
        extern const AttachmentDescription depthClearDontCareReadOnly;
        extern const AttachmentDescription depthDontCareDontCareReadOnly;

        // TODO: add depthStencil attachment descriptions
    } // namespace attachments
} // namespace magma
