#include "Cube.hpp"
#include "TexturesInfo.hpp"

#include <GL/gl.h>

namespace BraXaPsaIII {

Cube::Cube()
    : type(Type::INITAL)
    , isClicked(false)
{
}

void Cube::drawCube(const std::uint8_t cubeStencilId)
{
    bool isTextureEnabled = false;

    if (type == Type::INITAL) {
        glColor3f(0.98f, 0.92f, 0.82f);

        isTextureEnabled = false;
        glStencilMask(0x00);
    }

    else if (type == Type::EMPTY) {
        glColor3f(0.0f, 0.0f, 0.0f);

        isTextureEnabled = false;
        glStencilMask(0x00);
    }

    else if (type == Type::RED) {
        isClicked == false ? glColor3f(1.0f, 0.0f, 0.0f) : glColor3f(0.5f, 0.0f, 0.0f);

        isTextureEnabled = false;
        glStencilMask(0xFF);

    }

    else if (type == Type::BLUE) {
        isClicked == false ? glColor3f(0.0f, 0.0f, 1.0f) : glColor3f(0.0f, 0.0f, 0.5f);

        isTextureEnabled = false;
        glStencilMask(0xFF);

    }

    else if (type == Type::ROCK) {
        isClicked == false ? glColor3f(1.0f, 1.0f, 1.0f) : glColor3f(0.5f, 0.5f, 0.5f);

        isTextureEnabled = true;
        glBindTexture(GL_TEXTURE_2D, TexturesInfo::rock);
        glStencilMask(0xFF);
    }

    else if (type == Type::PAPER) {
        isClicked == false ? glColor3f(1.0f, 1.0f, 1.0f) : glColor3f(0.5f, 0.5f, 0.5f);

        isTextureEnabled = true;
        glBindTexture(GL_TEXTURE_2D, TexturesInfo::paper);
        glStencilMask(0xFF);

    }

    else if (type == Type::SCISSOR) {
        isClicked == false ? glColor3f(1.0f, 1.0f, 1.0f) : glColor3f(0.5f, 0.5f, 0.5f);

        isTextureEnabled = true;
        glBindTexture(GL_TEXTURE_2D, TexturesInfo::scissor);
        glStencilMask(0xFF);
    }

    if (isTextureEnabled == true) {
        glEnable(GL_TEXTURE_2D);
    }

    glStencilFunc(GL_ALWAYS, static_cast<GLint>(cubeStencilId), 0xFF);
    glBegin(GL_QUADS);

    const GLfloat texCoordsArr[4][2] = {
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f }
    };

    for (auto i = 0; i < std::size(s_faces); i++) {
        for (auto j = 0; j < std::size(s_faces[0]); j++) {
            glTexCoord2fv(texCoordsArr[j]);
            glVertex3fv(s_vertices[s_faces[i][j]]);
        }
    }

    glEnd();

    if (isTextureEnabled == true) {
        glDisable(GL_TEXTURE_2D);
    }
}

}
