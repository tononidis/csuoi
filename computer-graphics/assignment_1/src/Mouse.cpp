#include "Mouse.hpp"
#include "Cube.hpp"
#include "GameManagement.hpp"
#include "Utils.hpp"

#include <GL/glut.h>

#include <spdlog/spdlog.h>

#include <array>
#include <utility>

namespace BraXaPsaIII {

namespace Mouse {

    static void firstMouseEventHandler(int button, int state, int x, int y);
    static void secondMouseEventHandler(int button, int state, int x, int y);

    struct CubeClickedInfo {
        std::pair<Utils::Point, Cube*> firstCubeSelection;
        std::pair<Utils::Point, Cube*> secondCubeSelection;

        CubeClickedInfo()
            : firstCubeSelection({ { -1, -1 }, nullptr })
            , secondCubeSelection({ { -1, -1 }, nullptr })
        {
        }

        void reset()
        {
            if (firstCubeSelection.second != nullptr) {
                firstCubeSelection.second->isClicked = false;
            }

            if (secondCubeSelection.second != nullptr) {
                secondCubeSelection.second->isClicked = false;
            }

            firstCubeSelection = { { -1, -1 }, nullptr };
            secondCubeSelection = { { -1, -1 }, nullptr };
        }
    };

    static CubeClickedInfo s_g_cubeClickedInfo = CubeClickedInfo { };
    static std::uint8_t s_g_stencilRead = 0;

    static void clearClickStatus()
    {
        auto firstCubeSelection = s_g_cubeClickedInfo.firstCubeSelection.second;
        if (firstCubeSelection != nullptr && firstCubeSelection->isClicked == true) {
            SPDLOG_TRACE("Changing first cube select status from [true - > false] ...");

            firstCubeSelection->isClicked = false;
        }

        auto secondCubeSelection = s_g_cubeClickedInfo.secondCubeSelection.second;
        if (secondCubeSelection != nullptr && secondCubeSelection->isClicked == true) {
            SPDLOG_TRACE("Changing first cube select status from [true - > false] ...");

            secondCubeSelection->isClicked = false;
        }
    }

    static bool arePointsNeighbor(const Utils::Point& pointA, const Utils::Point& pointB)
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
        auto& firstPointSelection = s_g_cubeClickedInfo.firstCubeSelection.first;
        auto& secondPointSelection = s_g_cubeClickedInfo.secondCubeSelection.first;

        if (arePointsNeighbor(firstPointSelection, secondPointSelection) == true) {
            SPDLOG_DEBUG("Swapping cube [{}, {}], with cube [{}, {}] ...", firstPointSelection.x, firstPointSelection.y, secondPointSelection.x, secondPointSelection.y);

            auto firstCubeSelection = s_g_cubeClickedInfo.firstCubeSelection.second;
            auto secondCubeSelection = s_g_cubeClickedInfo.secondCubeSelection.second;

            if (firstCubeSelection == nullptr || secondCubeSelection == nullptr) {
                SPDLOG_ERROR("firstCubeSelection = [{}] and/or  secondCubeSelection = [{}] is NULL", firstCubeSelection == nullptr ? "null" : "", secondCubeSelection == nullptr ? "null" : "");
            }

            else {
                std::swap(firstCubeSelection->type, secondCubeSelection->type);
                GameManagement::checkPointAndDestroyCubes(firstPointSelection);
                GameManagement::checkPointAndDestroyCubes(secondPointSelection);
                GameManagement::count++;
            }
        }
    }

    static void secondMouseEventHandler(int button, int state, int x, int y)
    {
        SPDLOG_TRACE("Mouse button [{}] clicked, state [{}], (x, y) = [{}, {}]", button, state, x, y);

        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
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
                    return;
                }

                if (y >= GameManagement::cubes2dArr[0].size() && y < 0) {
                    SPDLOG_ERROR("Stencil reads outeofbounds cube at y = [{}]", y);
                    clearClickStatus();
                    return;
                }

                s_g_cubeClickedInfo.secondCubeSelection.first = { x, y };
                s_g_cubeClickedInfo.secondCubeSelection.second = &GameManagement::cubes2dArr[x][y];
                s_g_cubeClickedInfo.secondCubeSelection.second->isClicked = true;

                SPDLOG_DEBUG("Cube clicked (x, y) [{}, {}]", x, y);

                swapCubesIfAreNeighbor();
                clearClickStatus();

                glutPostRedisplay();
                glutMouseFunc(firstMouseEventHandler);
            }
        }
    }

    static void firstMouseEventHandler(int button, int state, int x, int y)
    {
        SPDLOG_TRACE("Mouse button [{}] clicked, state [{}], (x, y) = [{}, {}]", button, state, x, y);

        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
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
                    return;
                }

                if (y >= GameManagement::cubes2dArr[0].size() && y < 0) {
                    SPDLOG_ERROR("Stencil reads outeofbounds cube at y = [{}]", y);
                    clearClickStatus();
                    return;
                }

                s_g_cubeClickedInfo.firstCubeSelection.first = { x, y };
                s_g_cubeClickedInfo.firstCubeSelection.second = &GameManagement::cubes2dArr[x][y];
                s_g_cubeClickedInfo.firstCubeSelection.second->isClicked = true;

                SPDLOG_DEBUG("Cube clicked (x, y) [{}, {}]", x, y);

                glutPostRedisplay();
                glutMouseFunc(secondMouseEventHandler);
            }
        }
    }

    void registerMouse()
    {
        glutMouseFunc(firstMouseEventHandler);
    }
}

}