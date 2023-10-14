#pragma once

#include "glcorearb.h"

struct GlWrapperAPI
{
# include "gl_api.h"
};

using PFNGETGLPROC = void* (const char*);
void GetGlAPI(GlWrapperAPI* api, PFNGETGLPROC GetGLProc);
void InjectWrapperAPI(GlWrapperAPI* api);