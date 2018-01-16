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
#include "../shared.h"

namespace magma
{
    class LoadStoreOp
    {
    public:
        LoadStoreOp(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp):
            loadOp(loadOp), storeOp(storeOp) {}

    private:
        VkAttachmentLoadOp loadOp;
        VkAttachmentStoreOp storeOp;
        friend class Attachment;
    };

    class Attachment : public VkAttachmentDescription
    {
    public:
        Attachment(VkFormat format, 
            uint32_t sampleCount,
            const LoadStoreOp& op,
            const LoadStoreOp& stencilOp,   
            VkImageLayout finalLayout,
            VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED);
        Attachment(VkSampleCountFlagBits samples,
            VkAttachmentLoadOp loadOp,
            VkAttachmentStoreOp storeOp,
            VkAttachmentLoadOp stencilLoadOp,
            VkAttachmentStoreOp stencilStoreOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout); 
        Attachment(VkFormat format, const Attachment& predefined);
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
        extern const Attachment colorLoadStoreSamples1;
        extern const Attachment colorClearStoreSamples1;
        extern const Attachment colorDontCareStoreSamples1;
        extern const Attachment colorLoadDontCareSamples1;
        extern const Attachment colorClearDontCareSamples1;
        extern const Attachment colorDontCareDontCareSamples1;

        extern const Attachment colorLoadStoreSamples2;
        extern const Attachment colorClearStoreSamples2;
        extern const Attachment colorDontCareStoreSamples2;
        extern const Attachment colorLoadDontCareSamples2;
        extern const Attachment colorClearDontCareSamples2;
        extern const Attachment colorDontCareDontCareSamples2;

        extern const Attachment colorLoadStoreSamples4;
        extern const Attachment colorClearStoreSamples4;
        extern const Attachment colorDontCareStoreSamples4;
        extern const Attachment colorLoadDontCareSamples4;
        extern const Attachment colorClearDontCareSamples4;
        extern const Attachment colorDontCareDontCareSamples4;

        extern const Attachment colorLoadStoreSamples8;
        extern const Attachment colorClearStoreSamples8;
        extern const Attachment colorDontCareStoreSamples8;
        extern const Attachment colorLoadDontCareSamples8;
        extern const Attachment colorClearDontCareSamples8;
        extern const Attachment colorDontCareDontCareSamples8;
    } // namespace attachments
} // namespace magma
