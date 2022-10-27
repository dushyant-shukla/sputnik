#pragma once

namespace sputnik
{

class Window
{

public:
    struct WindowSpecification
    {
        unsigned int m_width;
        unsigned int m_height;
        std::string  m_title;
    };

protected:
    // protected methods

private:
    // private methods

private:
    // private data

    WindowSpecification m_window_specification;
};

} // namespace sputnik