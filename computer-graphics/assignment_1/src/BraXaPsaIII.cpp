// Stefanos Tononidis 2833
// Panagiotis Drivas 2690

#include "Config.hpp"
#include "Display.hpp"
#include "GameManagement.hpp"
#include "Keyboard.hpp"
#include "Menu.hpp"
#include "Mouse.hpp"
#include "TexturesInfo.hpp"
#include "WindowUtils.hpp"

#include <GL/glut.h>

#include <spdlog/spdlog.h>

int main(int argc, char** argv)
{
    using namespace BraXaPsaIII;

    spdlog::flush_on(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S][%s:%!:%#][%l] %v");
    spdlog::set_level(spdlog::level::debug);

    WindowUtils::initializeWindow(&argc, argv);

    TexturesInfo::loadTextures();

    Display::registerDisplay();
    Mouse::registerMouse();
    Keyboard::registerKeyboard();
    Menu::registerMenu();

    glutMainLoop();

    return 0;
}