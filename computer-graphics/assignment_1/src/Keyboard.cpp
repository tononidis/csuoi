#include "Keyboard.hpp"

#ifdef _WIN32
#include <GL/glut.h>

#include <windows.h>
#else
#include <GL/glut.h>
#endif

#include <spdlog/spdlog.h>

namespace BraXaPsaIII {

namespace Keyboard {

static void keyboardEventHandler(unsigned char key, int x, int y)
{
    static constexpr uint8_t esc_key = 27;

    if (key == esc_key) {
        SPDLOG_INFO("Esc pressed detected exiting the game ...");
        std::exit(EXIT_SUCCESS);
    }
}

static void keyboardSpecialKeyEventHandler(int key, int x, int y)
{
    static constexpr auto step = 0.01;
    static auto deltaX = 0.0f;
    static auto deltaY = 0.0f;

    switch (key) {

    case GLUT_KEY_RIGHT: {
        deltaX -= step;
        break;
    }

    case GLUT_KEY_LEFT: {
        deltaX += step;
        break;
    }

    case GLUT_KEY_UP: {
        deltaY -= step;
        break;
    }

    case GLUT_KEY_DOWN: {
        deltaY += step;
        break;
    }

    default: {
        break;
    }
    }
}

void registerKeyboard()
{
    glutKeyboardFunc(keyboardEventHandler);
    glutSpecialFunc(keyboardSpecialKeyEventHandler);
}
} // namespace Keyboard

} // namespace BraXaPsaIII