#pragma once

#include "core/core.h"

namespace sputnik::glcore
{

class IndexBuffer
{

public:
    IndexBuffer();
    ~IndexBuffer();
    NON_COPYABLE(IndexBuffer);

    void Set(unsigned int* input_array, unsigned int length);
    void Set(std::vector<unsigned int>& input);

    const unsigned int& GetCount();
    const unsigned int& GetId();

protected:
    unsigned int m_id;
    unsigned int m_count;
};

} // namespace sputnik::glcore