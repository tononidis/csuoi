#include "GameManagement.hpp"

#include <GL/glut.h>

#include <spdlog/spdlog.h>

#include <random>

namespace BraXaPsaIII {

namespace GameManagement {

    struct CubeRefPointPair {
        Point point;
        Cube& cube;

        CubeRefPointPair(decltype(Point::x) x, decltype(Point::y) y)
            : point(x, y)
            , cube(cubes2dArr[x][y])
        {
        }
    };

    Cube::Type generateRandomSelectableType()
    {
        static std::mt19937 s_generator(std::random_device { }());
        static std::uniform_int_distribution<std::size_t> s_distribution(0, std::size(Cube::s_selectableTypes) - 1);

        return Cube::s_selectableTypes[s_distribution(s_generator)];
    }

    Cube::Type generateRandomSelectableTypeExclude(Cube::Type excludedType)
    {
        auto type = generateRandomSelectableType();
        while (type == excludedType) {
            type = generateRandomSelectableType();
        }

        return type;
    }

    static void addSecondAndThirdLevelDestructibleNeighbors(std::vector<Point>& neighborPoints_out, const Point& cubePoint)
    {
        Cube::Type currentType = cubes2dArr[cubePoint.x][cubePoint.y].type;

        auto tryAddNeighbor = [&](const Point& neighborPoint) {
            if (neighborPoint.isValid() == true) {
                auto& neighborCube = cubes2dArr[neighborPoint.x][neighborPoint.y];
                if (neighborCube.type == Cube::type_to_beaten_type(currentType)) {
                    SPDLOG_TRACE("Exploaded cube (x, y) = [{}, {}] with type = [{}], neighbor cube (x, y) = [{}, {}] with type = [{}] gets destroyed",
                        cubePoint.x,
                        cubePoint.y,
                        Cube::type_to_string(currentType),
                        neighborPoint.x,
                        neighborPoint.y,
                        Cube::type_to_string(neighborCube.type));
                    neighborPoints_out.emplace_back(neighborPoint.x, neighborPoint.y);
                }
            }
        };

        tryAddNeighbor(cubePoint.right(2));
        tryAddNeighbor(cubePoint.left(2));
        tryAddNeighbor(cubePoint.up(2));
        tryAddNeighbor(cubePoint.down(2));

        tryAddNeighbor(cubePoint.upLeft(2));
        tryAddNeighbor(cubePoint.upRight(2));
        tryAddNeighbor(cubePoint.downLeft(2));
        tryAddNeighbor(cubePoint.downRight(2));

        tryAddNeighbor(cubePoint.right(3));
        tryAddNeighbor(cubePoint.left(3));
        tryAddNeighbor(cubePoint.up(3));
        tryAddNeighbor(cubePoint.down(3));

        tryAddNeighbor(cubePoint.upLeft(3));
        tryAddNeighbor(cubePoint.upRight(3));
        tryAddNeighbor(cubePoint.downLeft(3));
        tryAddNeighbor(cubePoint.downRight(3));
    }

    static void addFirstLevelDestructibleNeighbors(std::vector<Point>& neighborPoints_out, const Point& cubePoint)
    {
        Cube::Type currentType = cubes2dArr[cubePoint.x][cubePoint.y].type;

        auto tryAddNeighbor = [&](const Point& neighborPoint) {
            if (neighborPoint.isValid() == true) {
                auto& neighborCube = cubes2dArr[neighborPoint.x][neighborPoint.y];
                if (neighborCube.type != Cube::type_to_beat_type(currentType)) {
                    SPDLOG_TRACE("Exploaded cube (x, y) = [{}, {}] with type = [{}], neighbor cube (x, y) = [{}, {}] with type = [{}] gets destroyed",
                        cubePoint.x,
                        cubePoint.y,
                        Cube::type_to_string(currentType),
                        neighborPoint.x,
                        neighborPoint.y,
                        Cube::type_to_string(neighborCube.type));
                    neighborPoints_out.emplace_back(neighborPoint.x, neighborPoint.y);
                }
            }
        };

        tryAddNeighbor(cubePoint.right(1));
        tryAddNeighbor(cubePoint.left(1));
        tryAddNeighbor(cubePoint.up(1));
        tryAddNeighbor(cubePoint.down(1));

        tryAddNeighbor(cubePoint.upLeft(1));
        tryAddNeighbor(cubePoint.upRight(1));
        tryAddNeighbor(cubePoint.downLeft(1));
        tryAddNeighbor(cubePoint.downRight(1));
    }

    static void addSpecialDestructibleCubePoints(std::vector<Point>& destructiblePoints_out, const Point& cubePoint)
    {
        Cube::Type currentType = cubes2dArr[cubePoint.x][cubePoint.y].type;

        if (currentType == Cube::Type::ROCK || currentType == Cube::Type::PAPER || currentType == Cube::Type::SCISSOR) {
            addFirstLevelDestructibleNeighbors(destructiblePoints_out, cubePoint);
            addSecondAndThirdLevelDestructibleNeighbors(destructiblePoints_out, cubePoint);
        }
    }

    bool getDestructibleCubePoints(std::vector<Point>& destructiblePoints_out, bool isUsedForReplacement)
    {
        destructiblePoints_out.clear();

        for (auto row = 0; row < cubes2dArr.size(); row++) {
            for (auto column = 0; column < cubes2dArr[0].size(); column++) {

                auto& cube = cubes2dArr[row][column];
                auto type = cube.type;
                if (type == Cube::Type::EMPTY) {
                    SPDLOG_DEBUG("Cube at (x, y) = [{}, {}] is already empty", row, column);
                    continue;
                }

                std::vector<Point> horizontalDestructiblePoints = { Point { row, column } };
                std::vector<Point> verticalDestructiblePoints = { Point { row, column } };

                if (row != cubes2dArr[0].size() - 1) {
                    auto rightCubeXPoint = row + 1;
                    while (rightCubeXPoint < cubes2dArr[0].size()) {
                        auto& rightCube = cubes2dArr[rightCubeXPoint][column];
                        if (rightCube.type == type) {
                            horizontalDestructiblePoints.emplace_back(rightCubeXPoint, column);
                            rightCubeXPoint++;
                        }

                        else {
                            break;
                        }
                    }
                }

                if (row != 0) {
                    auto leftCubeXPoint = row - 1;
                    while (leftCubeXPoint >= 0) {
                        auto& leftCube = cubes2dArr[leftCubeXPoint][column];
                        if (leftCube.type == type) {
                            horizontalDestructiblePoints.emplace_back(leftCubeXPoint, column);
                            leftCubeXPoint--;
                        }

                        else {
                            break;
                        }
                    }
                }

                if (column != cubes2dArr.size() - 1) {
                    auto upCubeYPoint = column + 1;
                    while (upCubeYPoint < cubes2dArr[0].size()) {
                        auto& rightCube = cubes2dArr[row][upCubeYPoint];
                        if (rightCube.type == type) {
                            verticalDestructiblePoints.emplace_back(row, upCubeYPoint);
                            upCubeYPoint++;
                        }

                        else {
                            break;
                        }
                    }
                }

                if (column != 0) {
                    auto downCubeYPoint = column - 1;
                    while (downCubeYPoint >= 0) {
                        auto& rightCube = cubes2dArr[row][downCubeYPoint];
                        if (rightCube.type == type) {
                            verticalDestructiblePoints.emplace_back(row, downCubeYPoint);
                            downCubeYPoint--;
                        }

                        else {
                            break;
                        }
                    }
                }

                if (horizontalDestructiblePoints.size() >= 3) {
                    destructiblePoints_out.insert(destructiblePoints_out.end(), horizontalDestructiblePoints.begin(), horizontalDestructiblePoints.end());
                }

                if (verticalDestructiblePoints.size() >= 3) {
                    destructiblePoints_out.insert(destructiblePoints_out.end(), verticalDestructiblePoints.begin(), verticalDestructiblePoints.end());
                }
            }
        }

        if (destructiblePoints_out.size() >= 3) {
            std::sort(destructiblePoints_out.begin(), destructiblePoints_out.end());
            destructiblePoints_out.erase(std::unique(destructiblePoints_out.begin(), destructiblePoints_out.end()), destructiblePoints_out.end());

            if (isUsedForReplacement != true) {
                std::vector<Point> specialDestructiblePoints;
                for (const auto& cubePoint : destructiblePoints_out) {
                    addSpecialDestructibleCubePoints(specialDestructiblePoints, cubePoint);
                }

                if (specialDestructiblePoints.size() > 0) {
                    destructiblePoints_out.insert(destructiblePoints_out.end(), specialDestructiblePoints.begin(), specialDestructiblePoints.end());
                    std::sort(destructiblePoints_out.begin(), destructiblePoints_out.end());
                    destructiblePoints_out.erase(std::unique(destructiblePoints_out.begin(), destructiblePoints_out.end()), destructiblePoints_out.end());
                }
            }

            return true;
        }

        else {
            return false;
        }
    }

    void replaceEmptyCubesWithOutScoring(void)
    {
        std::vector<Point> destructiblePoints;

        bool destructibleCubePointsExist = getDestructibleCubePoints(destructiblePoints, true);
        while (destructibleCubePointsExist == true) {
            for (const auto& point : destructiblePoints) {
                cubes2dArr[point.x][point.y].type = generateRandomSelectableTypeExclude(cubes2dArr[point.x][point.y].type);
            }

            destructibleCubePointsExist = getDestructibleCubePoints(destructiblePoints, true);
        }
    }

    void createNewGame(void)
    {
        for (auto row = 0; row < cubes2dArr.size(); row++) {
            for (auto column = 0; column < cubes2dArr[0].size(); column++) {
                cubes2dArr[row][column].type = generateRandomSelectableType();

                SPDLOG_TRACE("First pass assigning cube (x, y) = [{}, {}] with type = [{}]", row, column, Cube::type_to_string(cubes2dArr[row][column].type));
            }
        }

        replaceEmptyCubesWithOutScoring();
        glutPostRedisplay();
    }
}
}