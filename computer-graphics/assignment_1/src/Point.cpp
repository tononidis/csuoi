#include "Point.hpp"
#include "Config.hpp"

#include <cassert>

namespace BraXaPsaIII {

Point::Point(int x_in, int y_in)
    : x(x_in)
    , y(y_in)
{
}

bool Point::isValid() const
{
    return (x >= 0 && x < Config::GRID_SIZE) && (y >= 0 && y < Config::GRID_SIZE);
}

Point Point::right(int step) const
{
    assert(step >= 1);
    return Point { x + step, y };
}

Point Point::left(int step) const
{
    assert(step >= 1);
    return Point { x - step, y };
}

Point Point::up(int step) const
{
    assert(step >= 1);
    return Point { x, y + step };
}

Point Point::down(int step) const
{
    assert(step >= 1);
    return Point { x, y - step };
}

Point Point::upRight(int step) const
{
    assert(step >= 1);
    return Point { x + step, y + step };
}

Point Point::upLeft(int step) const
{
    assert(step >= 1);
    return Point { x - step, y + step };
}

Point Point::downRight(int step) const
{
    assert(step >= 1);
    return Point { x + step, y - step };
}

Point Point::downLeft(int step) const
{
    assert(step >= 1);
    return Point { x - step, y - step };
}

bool Point::operator<(const Point& other) const
{
    assert(other.isValid() == true);
    if (y != other.y) {
        return y < other.y;
    }
    return x < other.x;
}

bool Point::operator==(const Point& other) const
{
    assert(other.isValid() == true);
    return x == other.x && y == other.y;
}

}
