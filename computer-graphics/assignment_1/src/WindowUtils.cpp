#include "WindowUtils.hpp"
#include "Config.hpp"

#include <GL/glut.h>

namespace BraXaPsaIII {

namespace WindowUtils {

    static void doPostRedisplay(int)
    {
        glutPostRedisplay();
    }

    static void reshapeWindowEventHandler(int width, int height)
    {
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.0f, 3.0f);
        glutTimerFunc(100, doPostRedisplay, 0);
    }

    void initializeWindow(int* argcp, char** argv)
    {
        glutInit(argcp, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
        glutInitWindowSize(Config::START_WIDTH, Config::START_HEIGHT);
        glutInitWindowPosition(Config::WINDOW_START_POS_X, Config::WINDOW_START_POS_Y);
        glutCreateWindow(Config::WINDOW_NAME);
        glutReshapeFunc(reshapeWindowEventHandler);
    }

}

};