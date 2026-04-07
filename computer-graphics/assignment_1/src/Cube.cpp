#include "Cube.hpp"
#include "TexturesInfo.hpp"

#include <spdlog/spdlog.h>

namespace BraXaPsaIII {

Cube::Cube()
    : type(Type::INITAL)
    , isClicked(false)

{
}

void Cube::drawCube(const std::uint8_t&& cubeStencilId)
{
    static GLfloat s_color[3] = { 0.0f, 0.0f, 0.0f };
    static bool s_isTextureEnabled = false;

    if (type == Type::INITAL) {
        s_color[0] = 0.98;
        s_color[1] = 0.92;
        s_color[2] = 0.82;

        s_isTextureEnabled = false;
        glStencilMask(0x00);
    }

    else if (type == Type::EMPTY) {
        s_color[0] = 0.0f;
        s_color[1] = 0.0f;
        s_color[2] = 0.0f;

        s_isTextureEnabled = false;
        glStencilMask(0x00);
    }

    else if (type == Type::RED) {
        s_color[0] = isClicked == false ? 1.0f : 0.5f;
        s_color[1] = isClicked == false ? 0.0f : 0.0f;
        s_color[2] = isClicked == false ? 0.0f : 0.0f;

        s_isTextureEnabled = false;
        glStencilMask(0xFF);

    }

    else if (type == Type::BLUE) {
        s_color[0] = isClicked == false ? 0.0f : 0.0f;
        s_color[1] = isClicked == false ? 0.0f : 0.0f;
        s_color[2] = isClicked == false ? 1.0f : 0.5f;

        s_isTextureEnabled = false;
        glStencilMask(0xFF);

    }

    else if (type == Type::ROCK) {
        s_color[0] = isClicked == false ? 1.0f : 0.5f;
        s_color[1] = isClicked == false ? 1.0f : 0.5f;
        s_color[2] = isClicked == false ? 1.0f : 0.5f;

        s_isTextureEnabled = true;
        glBindTexture(GL_TEXTURE_2D, TexturesInfo::rock);
        glStencilMask(0xFF);
    }

    else if (type == Type::PAPER) {
        s_color[0] = isClicked == false ? 1.0f : 0.5f;
        s_color[1] = isClicked == false ? 1.0f : 0.5f;
        s_color[2] = isClicked == false ? 1.0f : 0.5f;

        s_isTextureEnabled = true;
        glBindTexture(GL_TEXTURE_2D, TexturesInfo::paper);
        glStencilMask(0xFF);

    }

    else if (type == Type::SCISSOR) {
        s_color[0] = isClicked == false ? 1.0f : 0.5f;
        s_color[1] = isClicked == false ? 1.0f : 0.5f;
        s_color[2] = isClicked == false ? 1.0f : 0.5f;

        s_isTextureEnabled = true;
        glBindTexture(GL_TEXTURE_2D, TexturesInfo::scissor);
        glStencilMask(0xFF);
    }

    if (s_isTextureEnabled == true) {
        glEnable(GL_TEXTURE_2D);
    }

    glStencilFunc(GL_ALWAYS, static_cast<GLint>(cubeStencilId), 0xFF);
    glBegin(GL_QUADS);

    glColor3f(s_color[0], s_color[1], s_color[2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[0][0]]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[0][1]]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[0][2]]);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[0][3]]);

    glColor3f(s_color[0], s_color[1], s_color[2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[1][0]]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[1][1]]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[1][2]]);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[1][3]]);

    glColor3f(s_color[0], s_color[1], s_color[2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[2][0]]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[2][1]]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[2][2]]);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[2][3]]);

    glColor3f(s_color[0], s_color[1], s_color[2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[3][0]]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[3][1]]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[3][2]]);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[3][3]]);

    glColor3f(s_color[0], s_color[1], s_color[2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[4][0]]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[4][1]]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[4][2]]);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[4][3]]);

    glColor3f(s_color[0], s_color[1], s_color[2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[5][0]]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[5][1]]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(s_vertices[s_faces[5][2]]);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(s_vertices[s_faces[5][3]]);

    glEnd();

    if (s_isTextureEnabled == true) {
        glDisable(GL_TEXTURE_2D);
    }
}

}
