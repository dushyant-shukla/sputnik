#ifndef PHX_BOUNDS_HPP
#define PHX_BOUNDS_HPP

#include "phx_math_utils.hpp"

namespace phx
{

class PhxExtent
{
public:
    PhxExtent();
    PhxExtent(const PhxVec3& min, const PhxVec3& max);
    PhxExtent(const PhxExtent& other);
    PhxExtent(PhxExtent&& other) noexcept;
    PhxExtent& operator=(const PhxExtent& other);
    PhxExtent& operator=(PhxExtent&& other) noexcept;

    void clear();
    bool isOverlapping(const PhxExtent& other) const;
    void expand(const PhxVec3* points, const PhxInt& num_points);
    void expand(const PhxVec3& other);
    void expand(const PhxExtent& other);

    PhxReal getWidthX() const;
    PhxReal getWidthY() const;
    PhxReal getWidthZ() const;

public:
    PhxVec3 m_min;
    PhxVec3 m_max;
};

} // namespace phx

#endif // !PHX_BOUNDS_HPP