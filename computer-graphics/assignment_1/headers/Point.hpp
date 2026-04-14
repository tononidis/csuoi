#pragma once

namespace BraXaPsaIII {

struct Point {
public:
    int x;
    int y;

    explicit Point(int x_in, int y_in);

    bool isValid() const;

    Point right(int step = 1) const;
    Point left(int step = 1) const;
    Point up(int step = 1) const;
    Point down(int step = 1) const;
    Point upLeft(int step = 1) const;
    Point upRight(int step = 1) const;
    Point downRight(int step = 1) const;
    Point downLeft(int step = 1) const;

    bool operator<(const Point& other) const;
    bool operator==(const Point& other) const;
};

}