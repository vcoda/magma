# Magma - Abstraction layer over Khronos Vulkan API.
# Copyright (C) 2018-2024 Victor Coda.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

INCLUDE_DIR=-I$(VULKAN_SDK)/include -I../../src -I../../src/core
CFLAGS=-std=c++14 -m64 -msse4 -DMAGMA_SSE $(INCLUDE_DIR)

unittest: unittest.cpp
	g++ $(CFLAGS) unittest.cpp -o unittest

clean:
	@rm -rf unittest

