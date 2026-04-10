#include "GameManagement.hpp"

#include <GL/glut.h>

#include <spdlog/spdlog.h>

#include <cstdlib>

namespace BraXaPsaIII {

namespace GameManagement {

    using cubePointPair = std::pair<Utils::Point, Cube&>;

    static void dropCubesToEmptySlot(const std::vector<Utils::Point>& pointsVec)
    {
        for (const auto& point : pointsVec) {
            SPDLOG_TRACE("Attempting to lift cube (x, y) = [{}, {}] ...", point.x, point.y);

            auto cubeYPoint = point.y;
            auto upCubeYPoint = point.y + 1;

            while (upCubeYPoint <= cubes2dArr[0].size() - 1) {
                SPDLOG_TRACE("Lifting cube (x, y) = [{}, {}] to (x, y) = [{}, {}]", point.x, cubeYPoint, point.x, upCubeYPoint);

                auto& cube = cubes2dArr[point.x][cubeYPoint];
                auto& upCube = cubes2dArr[point.x][upCubeYPoint];

                std::swap(cube.type, upCube.type);

                cubeYPoint += 1;
                upCubeYPoint += 1;
            }
        }
    }

    static void checkAndDestroyCubes(cubePointPair& cubeA, cubePointPair& cubeB, cubePointPair cubeC)
    {
        if (cubeA.second.type == cubeB.second.type && cubeA.second.type == cubeC.second.type && cubeA.second.type != Cube::Type::EMPTY) {

            cubeA.second.type = Cube::Type::EMPTY;
            cubeB.second.type = Cube::Type::EMPTY;
            cubeC.second.type = Cube::Type::EMPTY;

            std::vector<Utils::Point> pointsVec { cubeA.first, cubeB.first, cubeC.first };

            std::sort(pointsVec.begin(), pointsVec.end(), [](const Utils::Point& a, const Utils::Point& b) {
                return a.y > b.y;
            });

            dropCubesToEmptySlot(pointsVec);
        }
    }

    void checkPointAndDestroyCubes(const Utils::Point& point)
    {
        cubePointPair cube({ point.x, point.y }, GameManagement::cubes2dArr[point.x][point.y]);
        if (point.x == 0) {
            cubePointPair rightCube({ point.x + 1, point.y }, GameManagement::cubes2dArr[point.x + 1][point.y]);
            cubePointPair right2Cube({ point.x + 2, point.y }, GameManagement::cubes2dArr[point.x + 2][point.y]);

            checkAndDestroyCubes(cube, rightCube, right2Cube);
        }

        else if (point.x == 1) {
            cubePointPair leftCube({ point.x - 1, point.y }, GameManagement::cubes2dArr[point.x - 1][point.y]);
            cubePointPair rightCube({ point.x + 1, point.y }, GameManagement::cubes2dArr[point.x + 1][point.y]);
            cubePointPair right2Cube({ point.x + 2, point.y }, GameManagement::cubes2dArr[point.x + 2][point.y]);

            checkAndDestroyCubes(cube, rightCube, right2Cube);
            checkAndDestroyCubes(leftCube, cube, rightCube);
        }

        else if (point.x == GameManagement::cubes2dArr.size() - 1) {
            cubePointPair leftCube({ point.x - 1, point.y }, GameManagement::cubes2dArr[point.x - 1][point.y]);
            cubePointPair left2Cube({ point.x - 2, point.y }, GameManagement::cubes2dArr[point.x - 2][point.y]);

            checkAndDestroyCubes(left2Cube, leftCube, cube);
        }

        else if (point.x == GameManagement::cubes2dArr.size() - 2) {
            cubePointPair rightCube({ point.x + 1, point.y }, GameManagement::cubes2dArr[point.x + 1][point.y]);
            cubePointPair leftCube({ point.x - 1, point.y }, GameManagement::cubes2dArr[point.x - 1][point.y]);
            cubePointPair left2Cube({ point.x - 2, point.y }, GameManagement::cubes2dArr[point.x - 2][point.y]);

            checkAndDestroyCubes(left2Cube, leftCube, cube);
            checkAndDestroyCubes(leftCube, cube, rightCube);
        }

        else {
            cubePointPair rightCube({ point.x + 1, point.y }, GameManagement::cubes2dArr[point.x + 1][point.y]);
            cubePointPair right2Cube({ point.x + 2, point.y }, GameManagement::cubes2dArr[point.x + 2][point.y]);
            cubePointPair leftCube({ point.x - 1, point.y }, GameManagement::cubes2dArr[point.x - 1][point.y]);
            cubePointPair left2Cube({ point.x - 2, point.y }, GameManagement::cubes2dArr[point.x - 2][point.y]);

            checkAndDestroyCubes(cube, rightCube, right2Cube);
            checkAndDestroyCubes(leftCube, cube, rightCube);
            checkAndDestroyCubes(left2Cube, leftCube, cube);
        }

        if (point.y == 0) {
            cubePointPair upCube({ point.x, point.y + 1 }, GameManagement::cubes2dArr[point.x][point.y + 1]);
            cubePointPair up2Cube({ point.x, point.y + 2 }, GameManagement::cubes2dArr[point.x][point.y + 2]);

            checkAndDestroyCubes(cube, upCube, up2Cube);
        }

        else if (point.y == 1) {
            cubePointPair downCube({ point.x, point.y - 1 }, GameManagement::cubes2dArr[point.x][point.y - 1]);
            cubePointPair upCube({ point.x, point.y + 1 }, GameManagement::cubes2dArr[point.x][point.y + 1]);
            cubePointPair up2Cube({ point.x, point.y + 2 }, GameManagement::cubes2dArr[point.x][point.y + 2]);

            checkAndDestroyCubes(cube, upCube, up2Cube);
            checkAndDestroyCubes(downCube, cube, upCube);
        }

        else if (point.y == GameManagement::cubes2dArr[0].size() - 1) {
            cubePointPair downCube({ point.x, point.y - 1 }, GameManagement::cubes2dArr[point.x][point.y - 1]);
            cubePointPair down2Cube({ point.x, point.y - 2 }, GameManagement::cubes2dArr[point.x][point.y - 2]);

            checkAndDestroyCubes(down2Cube, downCube, cube);
        }

        else if (point.y == GameManagement::cubes2dArr[0].size() - 2) {
            cubePointPair upCube({ point.x, point.y + 1 }, GameManagement::cubes2dArr[point.x][point.y + 1]);
            cubePointPair downCube({ point.x, point.y - 1 }, GameManagement::cubes2dArr[point.x][point.y - 1]);
            cubePointPair down2Cube({ point.x, point.y - 2 }, GameManagement::cubes2dArr[point.x][point.y - 2]);

            checkAndDestroyCubes(down2Cube, downCube, cube);
            checkAndDestroyCubes(downCube, cube, upCube);
        }

        else {
            cubePointPair upCube({ point.x, point.y + 1 }, GameManagement::cubes2dArr[point.x][point.y + 1]);
            cubePointPair up2Cube({ point.x, point.y + 2 }, GameManagement::cubes2dArr[point.x][point.y + 2]);
            cubePointPair downCube({ point.x, point.y - 1 }, GameManagement::cubes2dArr[point.x][point.y - 1]);
            cubePointPair down2Cube({ point.x, point.y - 2 }, GameManagement::cubes2dArr[point.x][point.y - 2]);

            checkAndDestroyCubes(cube, upCube, up2Cube);
            checkAndDestroyCubes(downCube, cube, upCube);
            checkAndDestroyCubes(down2Cube, downCube, cube);
        }
    }

    static bool createUniqueCubeTriplet(Cube& cubeUp, Cube& cubeLeft, Cube& cube, Cube& cubeDown, Cube& cubeRight)
    {

        bool changedTriplet = false;
        if (cube.type == cubeUp.type && cube.type == cubeDown.type) {
            auto newTypeIndex = (static_cast<std::uint32_t>(cube.type) % std::size(Cube::s_selectableTypes)) + 1;
            cube.type = static_cast<Cube::Type>(newTypeIndex);

            changedTriplet = true;
        }

        if (cube.type == cubeLeft.type && cube.type == cubeRight.type) {
            auto newTypeIndex = (static_cast<std::uint32_t>(cube.type) % std::size(Cube::s_selectableTypes)) + 1;
            cube.type = static_cast<Cube::Type>(newTypeIndex);

            changedTriplet = true;
        }

        return changedTriplet;
    }

    static bool createUniqueCubeTripletAtEdges(Cube& cubeLeftOrUp, Cube& cube, Cube& cubeRightOrDown)
    {
        if (cube.type == cubeLeftOrUp.type && cube.type == cubeRightOrDown.type) {
            auto newTypeIndex = (static_cast<std::uint32_t>(cube.type) % std::size(Cube::s_selectableTypes)) + 1;
            cube.type = static_cast<Cube::Type>(newTypeIndex);

            return true;
        }

        return false;
    }

    void createNewGame(void)
    {
        int randomNum = -1;

        for (auto row = 0; row < cubes2dArr.size(); row++) {
            for (auto column = 0; column < cubes2dArr[0].size(); column++) {
                randomNum = std::rand() % std::size(Cube::s_selectableTypes);
                cubes2dArr[row][column].type = Cube::s_selectableTypes[randomNum];

                SPDLOG_TRACE("First pass assigning cube (x, y) = [{}, {}] with type = [{}]", row, column, Cube::type_to_string(cubes2dArr[row][column].type));
            }
        }

        for (auto column = 1; column < cubes2dArr[0].size() - 1; column++) {
            auto& cube = cubes2dArr[0][column];
            auto& leftCube = cubes2dArr[0][column - 1];
            auto& rightCube = cubes2dArr[0][column + 1];

            if (createUniqueCubeTripletAtEdges(leftCube, cube, rightCube) == true) {
                SPDLOG_TRACE("Edge pass re-assigning cube (x, y) = [{}, {}] with type = [{}]", 0, column, Cube::type_to_string(cube->type));
            }

            cube = cubes2dArr[cubes2dArr.size() - 1][column];
            leftCube = cubes2dArr[cubes2dArr.size() - 1][column - 1];
            rightCube = cubes2dArr[cubes2dArr.size() - 1][column + 1];

            if (createUniqueCubeTripletAtEdges(leftCube, cube, rightCube) == true) {
                SPDLOG_TRACE("Edge pass re-assigning cube (x, y) = [{}, {}] with type = [{}]", cubes2dArr.size() - 1, column, Cube::type_to_string(cube->type));
            }
        }

        for (auto row = 1; row < cubes2dArr.size() - 1; row++) {
            auto& cube = cubes2dArr[row][0];
            auto& downCube = cubes2dArr[row - 1][0];
            auto& upCube = cubes2dArr[row + 1][0];

            if (createUniqueCubeTripletAtEdges(downCube, cube, upCube) == true) {
                SPDLOG_TRACE("Edge pass re-assigning cube (x, y) = [{}, {}] with type = [{}]", row, 0, Cube::type_to_string(cube->type));
            }

            cube = cubes2dArr[row][cubes2dArr[0].size() - 1];
            downCube = cubes2dArr[row - 1][cubes2dArr[0].size() - 1];
            upCube = cubes2dArr[row + 1][cubes2dArr[0].size() - 1];

            if (createUniqueCubeTripletAtEdges(downCube, cube, upCube) == true) {
                SPDLOG_TRACE("Edge pass re-assigning cube (x, y) = [{}, {}] with type = [{}]", row, cubes2dArr[0].size() - 1, Cube::type_to_string(cube->type));
            }
        }

        std::uint8_t numOfChangedTriplets = 0;
        do {
            numOfChangedTriplets = 0;
            for (auto row = 1; row < cubes2dArr.size() - 1; row++) {
                for (auto column = 1; column < cubes2dArr[0].size() - 1; column++) {
                    auto& cube = cubes2dArr[row][column];
                    auto& upCube = cubes2dArr[row + 1][column];
                    auto& leftCube = cubes2dArr[row][column - 1];
                    auto& downCube = cubes2dArr[row - 1][column];
                    auto& rightCube = cubes2dArr[row][column + 1];

                    bool res = static_cast<decltype(numOfChangedTriplets)>(createUniqueCubeTriplet(upCube, leftCube, cube, downCube, rightCube));

                    if (res == true) {
                        SPDLOG_TRACE("Inner pass re-assigning cube (x, y) = [{}, {}] with type = [{}]", row, column, Cube::type_to_string(cube->type));
                    }

                    numOfChangedTriplets += res;
                }
            }
        } while (numOfChangedTriplets != 0);

        glutPostRedisplay();
    }
}

}