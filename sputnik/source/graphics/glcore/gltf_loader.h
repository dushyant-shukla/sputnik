#pragma once

#include "core/core.h"

#include <cgltf.h>

namespace sputnik::gltf
{

class GltfLoader
{

public:
    NON_INSTATIABLE(GltfLoader);

    static cgltf_data* LoadFile(const char* path);
    static void        FreeFile(cgltf_data* data);
};

} // namespace sputnik::gltf