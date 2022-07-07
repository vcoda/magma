/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "colorBlendAttachmentState.h"

namespace magma
{
    /* Blending combines the incoming source fragment's R, G, B, and A values
       with the destination R, G, B, and A values of each sample stored in the framebuffer
       at the fragment's location. Blending is performed for each pixel sample,
       rather than just once for each fragment. */

    struct ColorBlendState : VkPipelineColorBlendStateCreateInfo
    {
        constexpr ColorBlendState() noexcept;
        constexpr ColorBlendState(const ColorBlendAttachmentState& attachment,
            bool logicOpEnable = false,
            VkLogicOp logicOp = VK_LOGIC_OP_CLEAR,
            const std::initializer_list<float>& blendConstants = {1.f, 1.f, 1.f, 1.f}) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const ColorBlendState&) const noexcept;
    };

    /* Multiple attachment color blend state takes care about array of blend attachment states and
       thereof is copyable, but not constexpr-constructible. */

    struct MultiColorBlendState final : ColorBlendState
    {
        explicit MultiColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
            const std::initializer_list<float>& blendConstants = {1.f, 1.f, 1.f, 1.f}) noexcept;
        MultiColorBlendState(const ColorBlendState&) noexcept;
        MultiColorBlendState(const MultiColorBlendState&) noexcept;
        MultiColorBlendState& operator=(const MultiColorBlendState&) noexcept;
        ~MultiColorBlendState();
        hash_t hash() const noexcept;
        bool operator==(const MultiColorBlendState&) const noexcept;
    };
} // namespace magma

#include "colorBlendState.inl"

namespace magma
{
    namespace renderstate
    {
        constexpr ColorBlendState dontWriteRgba(blendstate::writeNone);

        constexpr ColorBlendState dontBlendR(blendstate::writeR);
        constexpr ColorBlendState dontBlendG(blendstate::writeG);
        constexpr ColorBlendState dontBlendB(blendstate::writeB);
        constexpr ColorBlendState dontBlendA(blendstate::writeA);
        constexpr ColorBlendState dontBlendRg(blendstate::writeRg);
        constexpr ColorBlendState dontBlendRb(blendstate::writeRb);
        constexpr ColorBlendState dontBlendRa(blendstate::writeRa);
        constexpr ColorBlendState dontBlendGb(blendstate::writeGb);
        constexpr ColorBlendState dontBlendGa(blendstate::writeGa);
        constexpr ColorBlendState dontBlendBa(blendstate::writeBa);
        constexpr ColorBlendState dontBlendRgb(blendstate::writeRgb);
        constexpr ColorBlendState dontBlendRga(blendstate::writeRga);
        constexpr ColorBlendState dontBlendRba(blendstate::writeRba);
        constexpr ColorBlendState dontBlendGba(blendstate::writeGba);
        constexpr ColorBlendState dontBlendRgba(blendstate::writeRgba);

        constexpr ColorBlendState blendAddR(blendstate::addR);
        constexpr ColorBlendState blendAddG(blendstate::addG);
        constexpr ColorBlendState blendAddB(blendstate::addB);
        constexpr ColorBlendState blendAddA(blendstate::addA);
        constexpr ColorBlendState blendAddRg(blendstate::addRg);
        constexpr ColorBlendState blendAddRb(blendstate::addRb);
        constexpr ColorBlendState blendAddRa(blendstate::addRa);
        constexpr ColorBlendState blendAddGb(blendstate::addGb);
        constexpr ColorBlendState blendAddGa(blendstate::addGa);
        constexpr ColorBlendState blendAddBa(blendstate::addBa);
        constexpr ColorBlendState blendAddRgb(blendstate::addRgb);
        constexpr ColorBlendState blendAddRga(blendstate::addRga);
        constexpr ColorBlendState blendAddRba(blendstate::addRba);
        constexpr ColorBlendState blendAddGba(blendstate::addGba);
        constexpr ColorBlendState blendAddRgba(blendstate::addRgba);

        constexpr ColorBlendState blendSubtractR(blendstate::subtractR);
        constexpr ColorBlendState blendSubtractG(blendstate::subtractG);
        constexpr ColorBlendState blendSubtractB(blendstate::subtractB);
        constexpr ColorBlendState blendSubtractA(blendstate::subtractA);
        constexpr ColorBlendState blendSubtractRg(blendstate::subtractRg);
        constexpr ColorBlendState blendSubtractRb(blendstate::subtractRb);
        constexpr ColorBlendState blendSubtractRa(blendstate::subtractRa);
        constexpr ColorBlendState blendSubtractGb(blendstate::subtractGb);
        constexpr ColorBlendState blendSubtractGa(blendstate::subtractGa);
        constexpr ColorBlendState blendSubtractBa(blendstate::subtractBa);
        constexpr ColorBlendState blendSubtractRgb(blendstate::subtractRgb);
        constexpr ColorBlendState blendSubtractRga(blendstate::subtractRga);
        constexpr ColorBlendState blendSubtractRba(blendstate::subtractRba);
        constexpr ColorBlendState blendSubtractGba(blendstate::subtractGba);
        constexpr ColorBlendState blendSubtractRgba(blendstate::subtractRgba);

        constexpr ColorBlendState blendReverseSubtractR(blendstate::reverseSubtractR);
        constexpr ColorBlendState blendReverseSubtractG(blendstate::reverseSubtractG);
        constexpr ColorBlendState blendReverseSubtractB(blendstate::reverseSubtractB);
        constexpr ColorBlendState blendReverseSubtractA(blendstate::reverseSubtractA);
        constexpr ColorBlendState blendReverseSubtractRg(blendstate::reverseSubtractRg);
        constexpr ColorBlendState blendReverseSubtractRb(blendstate::reverseSubtractRb);
        constexpr ColorBlendState blendReverseSubtractRa(blendstate::reverseSubtractRa);
        constexpr ColorBlendState blendReverseSubtractGb(blendstate::reverseSubtractGb);
        constexpr ColorBlendState blendReverseSubtractGa(blendstate::reverseSubtractGa);
        constexpr ColorBlendState blendReverseSubtractBa(blendstate::reverseSubtractBa);
        constexpr ColorBlendState blendReverseSubtractRgb(blendstate::reverseSubtractRgb);
        constexpr ColorBlendState blendReverseSubtractRga(blendstate::reverseSubtractRga);
        constexpr ColorBlendState blendReverseSubtractRba(blendstate::reverseSubtractRba);
        constexpr ColorBlendState blendReverseSubtractGba(blendstate::reverseSubtractGba);
        constexpr ColorBlendState blendReverseSubtractRgba(blendstate::reverseSubtractRgba);

        constexpr ColorBlendState blendMinR(blendstate::minR);
        constexpr ColorBlendState blendMinG(blendstate::minG);
        constexpr ColorBlendState blendMinB(blendstate::minB);
        constexpr ColorBlendState blendMinA(blendstate::minA);
        constexpr ColorBlendState blendMinRg(blendstate::minRg);
        constexpr ColorBlendState blendMinRb(blendstate::minRb);
        constexpr ColorBlendState blendMinRa(blendstate::minRa);
        constexpr ColorBlendState blendMinGb(blendstate::minGb);
        constexpr ColorBlendState blendMinGa(blendstate::minGa);
        constexpr ColorBlendState blendMinBa(blendstate::minBa);
        constexpr ColorBlendState blendMinRgb(blendstate::minRgb);
        constexpr ColorBlendState blendMinRga(blendstate::minRga);
        constexpr ColorBlendState blendMinRba(blendstate::minRba);
        constexpr ColorBlendState blendMinGba(blendstate::minGba);
        constexpr ColorBlendState blendMinRgba(blendstate::minRgba);

        constexpr ColorBlendState blendMaxR(blendstate::maxR);
        constexpr ColorBlendState blendMaxG(blendstate::maxG);
        constexpr ColorBlendState blendMaxB(blendstate::maxB);
        constexpr ColorBlendState blendMaxA(blendstate::maxA);
        constexpr ColorBlendState blendMaxRg(blendstate::maxRg);
        constexpr ColorBlendState blendMaxRb(blendstate::maxRb);
        constexpr ColorBlendState blendMaxRa(blendstate::maxRa);
        constexpr ColorBlendState blendMaxGb(blendstate::maxGb);
        constexpr ColorBlendState blendMaxGa(blendstate::maxGa);
        constexpr ColorBlendState blendMaxBa(blendstate::maxBa);
        constexpr ColorBlendState blendMaxRgb(blendstate::maxRgb);
        constexpr ColorBlendState blendMaxRga(blendstate::maxRga);
        constexpr ColorBlendState blendMaxRba(blendstate::maxRba);
        constexpr ColorBlendState blendMaxGba(blendstate::maxGba);
        constexpr ColorBlendState blendMaxRgba(blendstate::maxRgba);

        constexpr ColorBlendState blendMultiplyR(blendstate::multiplyR);
        constexpr ColorBlendState blendMultiplyG(blendstate::multiplyG);
        constexpr ColorBlendState blendMultiplyB(blendstate::multiplyB);
        constexpr ColorBlendState blendMultiplyA(blendstate::multiplyA);
        constexpr ColorBlendState blendMultiplyRg(blendstate::multiplyRg);
        constexpr ColorBlendState blendMultiplyRb(blendstate::multiplyRb);
        constexpr ColorBlendState blendMultiplyRa(blendstate::multiplyRa);
        constexpr ColorBlendState blendMultiplyGb(blendstate::multiplyGb);
        constexpr ColorBlendState blendMultiplyGa(blendstate::multiplyGa);
        constexpr ColorBlendState blendMultiplyBa(blendstate::multiplyBa);
        constexpr ColorBlendState blendMultiplyRgb(blendstate::multiplyRgb);
        constexpr ColorBlendState blendMultiplyRga(blendstate::multiplyRga);
        constexpr ColorBlendState blendMultiplyRba(blendstate::multiplyRba);
        constexpr ColorBlendState blendMultiplyGba(blendstate::multiplyGba);
        constexpr ColorBlendState blendMultiplyRgba(blendstate::multiplyRgba);

        constexpr ColorBlendState blendNormalR(blendstate::normalR);
        constexpr ColorBlendState blendNormalG(blendstate::normalG);
        constexpr ColorBlendState blendNormalB(blendstate::normalB);
        constexpr ColorBlendState blendNormalA(blendstate::normalA);
        constexpr ColorBlendState blendNormalRg(blendstate::normalRg);
        constexpr ColorBlendState blendNormalRb(blendstate::normalRb);
        constexpr ColorBlendState blendNormalRa(blendstate::normalRa);
        constexpr ColorBlendState blendNormalGb(blendstate::normalGb);
        constexpr ColorBlendState blendNormalGa(blendstate::normalGa);
        constexpr ColorBlendState blendNormalBa(blendstate::normalBa);
        constexpr ColorBlendState blendNormalRgb(blendstate::normalRgb);
        constexpr ColorBlendState blendNormalRga(blendstate::normalRga);
        constexpr ColorBlendState blendNormalRba(blendstate::normalRba);
        constexpr ColorBlendState blendNormalGba(blendstate::normalGba);
        constexpr ColorBlendState blendNormalRgba(blendstate::normalRgba);

        constexpr ColorBlendState blendNormalPremultipliedR(blendstate::normalPremultipliedR);
        constexpr ColorBlendState blendNormalPremultipliedG(blendstate::normalPremultipliedG);
        constexpr ColorBlendState blendNormalPremultipliedB(blendstate::normalPremultipliedB);
        constexpr ColorBlendState blendNormalPremultipliedA(blendstate::normalPremultipliedA);
        constexpr ColorBlendState blendNormalPremultipliedRg(blendstate::normalPremultipliedRg);
        constexpr ColorBlendState blendNormalPremultipliedRb(blendstate::normalPremultipliedRb);
        constexpr ColorBlendState blendNormalPremultipliedRa(blendstate::normalPremultipliedRa);
        constexpr ColorBlendState blendNormalPremultipliedGb(blendstate::normalPremultipliedGb);
        constexpr ColorBlendState blendNormalPremultipliedGa(blendstate::normalPremultipliedGa);
        constexpr ColorBlendState blendNormalPremultipliedBa(blendstate::normalPremultipliedBa);
        constexpr ColorBlendState blendNormalPremultipliedRgb(blendstate::normalPremultipliedRgb);
        constexpr ColorBlendState blendNormalPremultipliedRga(blendstate::normalPremultipliedRga);
        constexpr ColorBlendState blendNormalPremultipliedRba(blendstate::normalPremultipliedRba);
        constexpr ColorBlendState blendNormalPremultipliedGba(blendstate::normalPremultipliedGba);
        constexpr ColorBlendState blendNormalPremultipliedRgba(blendstate::normalPremultipliedRgba);

        constexpr ColorBlendState blendScreenR(blendstate::screenR);
        constexpr ColorBlendState blendScreenG(blendstate::screenG);
        constexpr ColorBlendState blendScreenB(blendstate::screenB);
        constexpr ColorBlendState blendScreenA(blendstate::screenA);
        constexpr ColorBlendState blendScreenRg(blendstate::screenRg);
        constexpr ColorBlendState blendScreenRb(blendstate::screenRb);
        constexpr ColorBlendState blendScreenRa(blendstate::screenRa);
        constexpr ColorBlendState blendScreenGb(blendstate::screenGb);
        constexpr ColorBlendState blendScreenGa(blendstate::screenGa);
        constexpr ColorBlendState blendScreenBa(blendstate::screenBa);
        constexpr ColorBlendState blendScreenRgb(blendstate::screenRgb);
        constexpr ColorBlendState blendScreenRga(blendstate::screenRga);
        constexpr ColorBlendState blendScreenRba(blendstate::screenRba);
        constexpr ColorBlendState blendScreenGba(blendstate::screenGba);
        constexpr ColorBlendState blendScreenRgba(blendstate::screenRgba);
    } // namespace renderstate
} // namespace magma
