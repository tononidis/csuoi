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

    bool getDestructibleCubePoints(std::vector<Point>& destructiblePoints_out)
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

                std::vector<Point> horizontalDestructiblePoints = { { row, column } };
                std::vector<Point> verticalDestructiblePoints = { { row, column } };

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

            return true;
        }

        else {
            return false;
        }
    }

    static void replaceEmptyCubes(void)
    {
        std::vector<Point> destructiblePoints;

        bool destructibleCubePointsExist = getDestructibleCubePoints(destructiblePoints);
        while (destructibleCubePointsExist == true) {
            for (const auto& point : destructiblePoints) {
                cubes2dArr[point.x][point.y].type = generateRandomSelectableTypeExclude(cubes2dArr[point.x][point.y].type);
            }

            destructibleCubePointsExist = getDestructibleCubePoints(destructiblePoints);
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

        replaceEmptyCubes();
        glutPostRedisplay();
    }
}
}