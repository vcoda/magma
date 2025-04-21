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

cl /std:c++17 /EHsc /D MAGMA_SSE /I%VULKAN_SDK%\Include /I..\..\src\third-party\pfr\include /I..\..\src unittest.cpp /link
