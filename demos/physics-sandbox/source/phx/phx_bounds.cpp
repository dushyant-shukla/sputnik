#include "phx_bounds.hpp"

namespace phx
{

PhxExtent::PhxExtent()
{
    clear();
}

PhxExtent::PhxExtent(const PhxVec3& min, const PhxVec3& max) : m_min(min), m_max(max) {}

PhxExtent::PhxExtent(const PhxExtent& other) : m_min(other.m_min), m_max(other.m_max) {}

PhxExtent::PhxExtent(PhxExtent&& other) noexcept : m_min(std::move(other.m_min)), m_max(std::move(other.m_max)) {}

PhxExtent& PhxExtent::operator=(const PhxExtent& other)
{
    if(this != &other)
    {
        m_min = other.m_min;
        m_max = other.m_max;
    }
    return *this;
}

PhxExtent& PhxExtent::operator=(PhxExtent&& other) noexcept
{
    if(this != &other)
    {
        m_min = std::move(other.m_min);
        m_max = std::move(other.m_max);
    }
    return *this;
}

void PhxExtent::clear()
{
    m_min = PhxVec3(kPhxEpsilon, kPhxEpsilon, kPhxEpsilon);
    m_max = PhxVec3(kPhxEpsilon, kPhxEpsilon, kPhxEpsilon);
}

bool PhxExtent::isOverlapping(const PhxExtent& other) const
{
    if(m_max.x < other.m_min.x || m_max.y < other.m_min.y || m_max.z < other.m_min.z)
    {
        return false;
    }
    if(other.m_max.x < m_min.x || other.m_max.y < m_min.y || other.m_max.z < m_min.z)
    {
        return false;
    }
    return true;
}

void PhxExtent::expand(const PhxVec3* points, const PhxInt& num_points)
{
    for(PhxInt i = 0; i < num_points; ++i)
    {
        expand(points[i]);
    }
}

void PhxExtent::expand(const PhxVec3& point)
{
    if(point.x < m_min.x)
    {
        m_min.x = point.x;
    }
    if(point.y < m_min.y)
    {
        m_min.y = point.y;
    }
    if(point.z < m_min.z)
    {
        m_min.z = point.z;
    }
    if(point.x > m_max.x)
    {
        m_max.x = point.x;
    }
    if(point.y > m_max.y)
    {
        m_max.y = point.y;
    }
    if(point.z > m_max.z)
    {
        m_max.z = point.z;
    }
}

void PhxExtent::expand(const PhxExtent& other)
{
    expand(other.m_min);
    expand(other.m_max);
}

PhxReal PhxExtent::getWidthX() const
{
    return m_max.x - m_min.x;
}

PhxReal PhxExtent::getWidthY() const
{
    return m_max.y - m_min.y;
}

PhxReal PhxExtent::getWidthZ() const
{
    return m_max.z - m_min.z;
}

} // namespace phx