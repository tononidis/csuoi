#include "Display.hpp"
#include "Cube.hpp"
#include "GameManagement.hpp"

#include <GL/gl.h>
#include <GL/glut.h>

#include <spdlog/spdlog.h>

namespace BraXaPsaIII {

namespace Display {

    static void displayScore(void)
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(150, 600, 0, 600);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glLoadIdentity();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glRasterPos2i(300, 550);

        std::string scoreMsg = "Your score is: " + std::to_string(GameManagement::score);

        for (const auto& c : scoreMsg) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        glMatrixMode(GL_PROJECTION);

        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    static void displayEventHandler(void)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        displayScore();

        glMatrixMode(GL_MODELVIEW);

        glLoadIdentity();
        glTranslatef(-1.0f + Cube::s_size / 2.0f, -1.0f + Cube::s_size / 2.0f, -2.5f);

        for (auto row = 0; row < GameManagement::cubes2dArr.size(); row++) {
            for (auto column = 0; column < GameManagement::cubes2dArr[0].size(); column++) {
                GameManagement::cubes2dArr[row][column].drawCube(column + (row << 4) + 1);
                glTranslatef(0.0f, Cube::s_size * 2.0f, 0.0f);
            }
            glTranslatef(Cube::s_size * 2.0f, -Cube::s_size * 2.0f * Config::GRID_SIZE, 0.0f);
        }

        if (GameManagement::gameState == GameManagement::GameState::NEW_GAME) {
            GameManagement::destroyCubes();
        }

        glutSwapBuffers();
    }

    void registerDisplay()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        glDepthFunc(GL_LEQUAL);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glutDisplayFunc(displayEventHandler);
    }
}

}