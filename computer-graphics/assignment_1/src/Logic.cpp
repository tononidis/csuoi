#include "Logic.hpp"

#include "Effects.hpp"
#include "GameManagement.hpp"

#include <spdlog/spdlog.h>

namespace BraXaPsaIII {

namespace Logic {

static bool arePointsNeighbor(const Point &pointA, const Point &pointB)
{
    auto dx = std::abs(pointA.x - pointB.x);
    auto dy = std::abs(pointA.y - pointB.y);

    bool isHorizontalNeighbor = dx == 1 && dy == 0;
    bool isVerticalNeighbor = dx == 0 && dy == 1;

    SPDLOG_DEBUG("isHorizontalNeighbor [{}], isVerticalNeighbor [{}], isNeighbor [{}]", isHorizontalNeighbor,
                 isVerticalNeighbor, isHorizontalNeighbor || isVerticalNeighbor);

    return isHorizontalNeighbor || isVerticalNeighbor;
}

void swapNeighborCubes(std::pair<Point, Cube *> &cubeA, std::pair<Point, Cube *> &cubeB)
{
    auto &firstPointSelection = cubeA.first;
    auto &secondPointSelection = cubeB.first;

    if (arePointsNeighbor(firstPointSelection, secondPointSelection) == true) {
        SPDLOG_DEBUG("Swapping cube [{}, {}], with cube [{}, {}] ...", firstPointSelection.x, firstPointSelection.y,
                     secondPointSelection.x, secondPointSelection.y);

        auto firstCubeSelection = cubeA.second;
        auto secondCubeSelection = cubeB.second;

        if (firstCubeSelection == nullptr || secondCubeSelection == nullptr) {
            SPDLOG_ERROR("firstCubeSelection = [{}] and/or  secondCubeSelection = [{}] is NULL",
                         firstCubeSelection == nullptr ? "null" : "", secondCubeSelection == nullptr ? "null" : "");
        }

        else {
            std::swap(firstCubeSelection->type, secondCubeSelection->type);
            Effects::destroyCubes();
            GameManagement::count++;
        }
    }
}

} // namespace Logic

} // namespace BraXaPsaIII