#pragma once

#include "glcorearb.h"

struct GlWrapperAPI
{
#include "gl_api.h"
};

using PFNGETGLPROC = void*(const char*);
void GetGlAPI(GlWrapperAPI* api, PFNGETGLPROC GetGLProc);
void InjectWrapperAPI(GlWrapperAPI* api);

// Example usage:
// GlWrapperAPI gl;
// GetGlAPI(&gl, [](const char* name) { return (void*)wglGetProcAddress(name); });
// InjectWrapperAPI(&gl);
// gl.glEnable(GL_TEXTURE_2D); // Example function call
