#include "Mouse.hpp"
#include "Cube.hpp"
#include "GameManagement.hpp"

#include <GL/glut.h>

#include <spdlog/spdlog.h>

#include <array>
#include <utility>

namespace BraXaPsaIII {

namespace Mouse {

    static void firstMouseEventHandler(int button, int state, int x, int y);
    static void secondMouseEventHandler(int button, int state, int x, int y);

    struct Point {
        int x;
        int y;

        Point(int x_in, int y_in)
            : x(x_in)
            , y(y_in)
        {
        }
    };

    struct CubeClickedInfo {
        std::pair<Point, Cube*> firstCube;
        std::pair<Point, Cube*> secondCube;

        CubeClickedInfo()
            : firstCube({ { -1, -1 }, nullptr })
            , secondCube({ { -1, -1 }, nullptr })
        {
        }

        void reset()
        {
            if (firstCube.second != nullptr) {
                firstCube.second->isClicked = false;
            }

            if (secondCube.second != nullptr) {
                secondCube.second->isClicked = false;
            }

            firstCube = { { -1, -1 }, nullptr };
            secondCube = { { -1, -1 }, nullptr };
        }
    };

    static CubeClickedInfo s_g_cubeClickedInfo = CubeClickedInfo { };

    static std::uint8_t s_g_stencilRead = 0;

    static void clearClickStatus()
    {
        SPDLOG_DEBUG("clearClickStatus");
        auto firstCube = s_g_cubeClickedInfo.firstCube.second;
        if (firstCube != nullptr && firstCube->isClicked == true) {
            SPDLOG_TRACE("Changing first cube select status from [true - > false] ...");

            firstCube->isClicked = false;
        }

        auto secondCube = s_g_cubeClickedInfo.secondCube.second;
        if (secondCube != nullptr && secondCube->isClicked == true) {
            SPDLOG_TRACE("Changing first cube select status from [true - > false] ...");

            secondCube->isClicked = false;
        }
    }

    static bool arePointsNeighbor(const Point& pointA, const Point& pointB)
    {
        auto dx = std::abs(pointA.x - pointB.x);
        auto dy = std::abs(pointA.y - pointB.y);

        bool isHorizontalNeighbor = dx == 1 && dy == 0;
        bool isVerticalNeighbor = dx == 0 && dy == 1;

        SPDLOG_DEBUG("isHorizontalNeighbor [{}], isVerticalNeighbor [{}], isNeighbor [{}]", isHorizontalNeighbor, isVerticalNeighbor, isHorizontalNeighbor || isVerticalNeighbor);

        return isHorizontalNeighbor || isVerticalNeighbor;
    }

    static void swapCubesIfAreNeighbor()
    {
        auto& currentPoint = s_g_cubeClickedInfo.firstCube.first;
        auto& previousPoint = s_g_cubeClickedInfo.secondCube.first;

        if (arePointsNeighbor(currentPoint, previousPoint) == true) {
            SPDLOG_DEBUG("Swapping cube [{}, {}], with cube [{}, {}] ...", currentPoint.x, currentPoint.y, previousPoint.x, previousPoint.y);

            auto firstCube = s_g_cubeClickedInfo.firstCube.second;
            auto secondCube = s_g_cubeClickedInfo.secondCube.second;

            if (firstCube == nullptr || secondCube == nullptr) {
                SPDLOG_ERROR("firstCube = [{}] and/or  secondCube = [{}] is NULL", firstCube == nullptr ? "null" : "", secondCube == nullptr ? "null" : "");
            }

            else {
                std::swap(firstCube->type, secondCube->type);
                GameManagement::count++;
            }
        }
    }

    static void secondMouseEventHandler(int button, int state, int x, int y)
    {
        SPDLOG_DEBUG("Mouse button [{}] clicked, state [{}], (x, y) = [{}, {}]", button, state, x, y);

        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &s_g_stencilRead);
            SPDLOG_DEBUG("Stencil read [{}]", s_g_stencilRead);

            if (s_g_stencilRead == 0x00) {
                SPDLOG_TRACE("Mouse click on empty scene");
                clearClickStatus();
            }

            else {
                auto x = (s_g_stencilRead & 0xF0) >> 4;
                auto y = -((s_g_stencilRead & 0xF) - Config::GRID_SIZE);

                if (x >= GameManagement::cubes2dArr.size() && x < 0) {
                    SPDLOG_ERROR("Stencil reads outeofbounds cube at x = [{}]", x);
                    clearClickStatus();
                }

                else if (y >= GameManagement::cubes2dArr[0].size() && y < 0) {
                    SPDLOG_ERROR("Stencil reads outeofbounds cube at y = [{}]", y);
                    clearClickStatus();
                }

                else {
                    s_g_cubeClickedInfo.secondCube.first = { x, y };
                    s_g_cubeClickedInfo.secondCube.second = &GameManagement::cubes2dArr[x][y];
                    s_g_cubeClickedInfo.secondCube.second->isClicked = true;

                    SPDLOG_DEBUG("Cube clicked (x, y) [{}, {}]", x, y);

                    swapCubesIfAreNeighbor();
                    clearClickStatus();

                    glutPostRedisplay();
                    glutMouseFunc(firstMouseEventHandler);
                }
            }
        }
    }

    static void firstMouseEventHandler(int button, int state, int x, int y)
    {
        SPDLOG_DEBUG("Mouse button [{}] clicked, state [{}], (x, y) = [{}, {}]", button, state, x, y);

        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &s_g_stencilRead);
            SPDLOG_DEBUG("Stencil read [{}]", s_g_stencilRead);

            if (s_g_stencilRead == 0x00) {
                SPDLOG_TRACE("Mouse click on empty scene");
                clearClickStatus();
            }

            else {
                auto x = (s_g_stencilRead & 0xF0) >> 4;
                auto y = -((s_g_stencilRead & 0xF) - Config::GRID_SIZE);

                if (x >= GameManagement::cubes2dArr.size() && x < 0) {
                    SPDLOG_ERROR("Stencil reads outeofbounds cube at x = [{}]", x);
                    clearClickStatus();
                }

                else if (y >= GameManagement::cubes2dArr[0].size() && y < 0) {
                    SPDLOG_ERROR("Stencil reads outeofbounds cube at y = [{}]", y);
                    clearClickStatus();
                }

                else {
                    s_g_cubeClickedInfo.firstCube.first = { x, y };
                    s_g_cubeClickedInfo.firstCube.second = &GameManagement::cubes2dArr[x][y];
                    s_g_cubeClickedInfo.firstCube.second->isClicked = true;

                    SPDLOG_DEBUG("Cube clicked (x, y) [{}, {}]", x, y);

                    glutPostRedisplay();
                    glutMouseFunc(secondMouseEventHandler);
                }
            }
        }
    }

    void registerMouse()
    {
        glutMouseFunc(firstMouseEventHandler);
    }
}

}