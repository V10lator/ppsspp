// Copyright (c) 2017- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#include <wiiu/gx2/shaders_asm.h>
#undef ARRAY_SIZE

#include "GPU/Common/ShaderCommon.h"
#include "GPU/GX2/FragmentShaderGeneratorGX2.h"
#include "GPU/Vulkan/FragmentShaderGeneratorVulkan.h"
#include "GPU/GX2/GX2StaticShaders.h"
#include "GPU/ge_constants.h"

#include <wiiu/os/debug.h>

void GenerateFragmentShaderGX2(const FShaderID &id, GX2PixelShader *ps) {
	// TODO;
	*ps = STPshaderGX2;

	if (id.Bit(FS_BIT_CLEARMODE)) {
		*ps = clearPShaderGX2;
	} else if (id.Bit(FS_BIT_DO_TEXTURE) && id.Bit(FS_BIT_BGRA_TEXTURE) && (id.Bits(FS_BIT_TEXFUNC, 3) == GE_TEXFUNC_ADD)) {
		*ps = cTexPShaderGX2;
	}

#if 1
	DEBUG_STR(FragmentShaderDesc(id).c_str());
	char glslcode[16384];
	GenerateVulkanGLSLFragmentShader(id, glslcode);
	puts(glslcode);
#endif
}
