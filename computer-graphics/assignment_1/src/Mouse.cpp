#include "Mouse.hpp"

#include "GameManagement.hpp"
#include "Logic.hpp"

#include <windows.h>
#include <GL/glut.h>

#include <spdlog/spdlog.h>

#include <array>
#include <utility>

namespace BraXaPsaIII {

namespace Mouse {

static void firstMouseEventHandler(int button, int state, int x, int y);
static void secondMouseEventHandler(int button, int state, int x, int y);

static std::pair<Point, Cube *> s_g_firstClickedCube = {Point{-1, -1}, nullptr};
static std::pair<Point, Cube *> s_g_secondClickedCube = {Point{-1, -1}, nullptr};
static std::uint8_t s_g_stencilRead = 0;

static void clearClickStatus()
{
    auto firstCubeSelection = s_g_firstClickedCube.second;
    if (firstCubeSelection != nullptr && firstCubeSelection->isClicked == true) {
        SPDLOG_TRACE("Changing first cube select status from [true - > false] ...");

        firstCubeSelection->isClicked = false;
    }

    auto secondCubeSelection = s_g_secondClickedCube.second;
    if (secondCubeSelection != nullptr && secondCubeSelection->isClicked == true) {
        SPDLOG_TRACE("Changing first cube select status from [true - > false] ...");

        secondCubeSelection->isClicked = false;
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

            s_g_secondClickedCube.first = Point{x, y};
            s_g_secondClickedCube.second = &GameManagement::cubes2dArr[x][y];
            s_g_secondClickedCube.second->isClicked = true;

            SPDLOG_DEBUG("Cube clicked (x, y) [{}, {}]", x, y);

            Logic::swapNeighborCubes(s_g_firstClickedCube, s_g_secondClickedCube);
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

            s_g_firstClickedCube.first = Point{x, y};
            s_g_firstClickedCube.second = &GameManagement::cubes2dArr[x][y];
            s_g_firstClickedCube.second->isClicked = true;

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
} // namespace Mouse

} // namespace BraXaPsaIII