#include "Keyboard.hpp"

#include <GL/glut.h>

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
        switch (key) {
        case GLUT_KEY_LEFT: {
            break;
        }

        case GLUT_KEY_RIGHT: {
            break;
        }

        case GLUT_KEY_UP: {
            break;
        }

        case GLUT_KEY_DOWN: {
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
}

}