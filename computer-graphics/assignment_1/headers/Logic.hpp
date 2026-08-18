#pragma once

#include "Cube.hpp"
#include "Point.hpp"

namespace BraXaPsaIII {

namespace Logic {

void swapNeighborCubes(std::pair<Point, Cube *> &cubeA, std::pair<Point, Cube *> &cubeB);

} // namespace Logic

} // namespace BraXaPsaIII