#pragma once

namespace BraXaPsaIII {

struct Point {
    int x;
    int y;

    Point(int x_in, int y_in)
        : x(x_in)
        , y(y_in)
    {
    }

    bool operator<(const Point& other) const
    {
        if (y != other.y) {
            return y < other.y;
        }
        return x < other.x;
    }

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }
};

}