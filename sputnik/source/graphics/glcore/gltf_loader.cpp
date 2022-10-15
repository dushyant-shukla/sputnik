#include "pch.h"
#include "gltf_loader.h"

namespace sputnik::gltf
{

cgltf_data* GltfLoader::LoadFile(const char* path)
{
    cgltf_options options;
    memset(&options, 0, sizeof(cgltf_options));
    cgltf_data* data = NULL;

    cgltf_result result = cgltf_parse_file(&options, path, &data); // load the glTF data from file.
    if(result != cgltf_result_success)
    {
        // cout << "Could not load: " << path << "\n";
        return 0;
    }

    result = cgltf_load_buffers(&options, data, path); // load any external buffer data
    if(result != cgltf_result_success)
    {
        cgltf_free(data);
        // cout << "Could not load: " << path << "\n";
        return 0;
    }

    result = cgltf_validate(data); // validate the gltf file just loaded
    if(result != cgltf_result_success)
    {
        cgltf_free(data);
        // assert() // "Invalid file"
        return 0;
    }
    return data;
}

void GltfLoader::FreeFile(cgltf_data* data)
{
    if(data == 0)
    {
        // assert(); // cannot free null data
    }
    else
    {
        cgltf_free(data);
    }
}

} // namespace sputnik::gltf