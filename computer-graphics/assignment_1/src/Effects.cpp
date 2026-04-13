#include "Effects.hpp"
#include "GameManagement.hpp"

#include <GL/glut.h>

#include <spdlog/spdlog.h>

namespace BraXaPsaIII {

namespace Effects {

    static std::vector<Point> s_g_destructiblePoints;

    static void dropCubesToEmptySlots(const std::vector<Point>& pointsVec)
    {
        for (const auto& point : pointsVec) {
            SPDLOG_TRACE("Attempting to lift cube (x, y) = [{}, {}] ...", point.x, point.y);

            auto cubeYPoint = point.y;
            auto upCubeYPoint = point.y + 1;

            while (upCubeYPoint <= GameManagement::cubes2dArr[0].size() - 1) {
                SPDLOG_TRACE("Lifting cube (x, y) = [{}, {}] to (x, y) = [{}, {}]", point.x, cubeYPoint, point.x, upCubeYPoint);

                auto& cube = GameManagement::cubes2dArr[point.x][cubeYPoint];
                auto& upCube = GameManagement::cubes2dArr[point.x][upCubeYPoint];

                std::swap(cube.type, upCube.type);

                cubeYPoint += 1;
                upCubeYPoint += 1;
            }
        }
    }

    static void continueDestroyingCubes(int x)
    {
        SPDLOG_DEBUG("Continuing with cube destruction ...");
        destroyCubes();
    }

    static void destroyCubesEffect(int x)
    {
        for (const auto& point : s_g_destructiblePoints) {
            GameManagement::cubes2dArr[point.x][point.y].type = GameManagement::generateRandomSelectableType();
            SPDLOG_DEBUG("Replaced cube at (x, y) = [{}, {}] with type = [{}]", point.x, point.y, Cube::type_to_string(GameManagement::cubes2dArr[point.x][point.y].type));
        }

        dropCubesToEmptySlots(s_g_destructiblePoints);
        glutPostRedisplay();

        glutTimerFunc(2000, continueDestroyingCubes, 0);
    }

    void destroyCubes(void)
    {
        bool destructibleCubePointsExist = GameManagement::getDestructibleCubePoints(s_g_destructiblePoints);

        if (destructibleCubePointsExist == true) {
            for (const auto& point : s_g_destructiblePoints) {
                GameManagement::cubes2dArr[point.x][point.y].type = Cube::Type::EMPTY;
                SPDLOG_DEBUG("Destroyed cube at (x, y) = [{}, {}]", point.x, point.y);
            }

            glutTimerFunc(2000, destroyCubesEffect, 0);
            glutPostRedisplay();
        }
    }
}

}
