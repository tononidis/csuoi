#pragma once

#include "Config.hpp"

#include <GL/gl.h>

#include <cstdint>

namespace BraXaPsaIII {

struct Cube {
    static constexpr float size = 1.0f / Config::GRID_SIZE;

    static constexpr GLfloat s_vertices[8][3] = {
        { 0, 0, 0 }, // P0
        { 0, size, 0 }, // P1
        { size, size, 0 }, // P2
        { size, 0, 0 }, // P3
        { 0, 0, size }, // P4
        { size, 0, size }, // P5
        { size, size, size }, // P6
        { 0, size, size } // P7
    };

    static constexpr std::uint8_t s_faces[6][4] = {
        { 0, 1, 2, 3 }, // Front
        { 4, 5, 6, 7 }, // Back
        { 1, 7, 6, 2 }, // Top
        { 0, 3, 5, 4 }, // Bottom
        { 0, 4, 7, 1 }, // Left
        { 3, 2, 6, 5 } // Right
    };

    enum struct Type {
        INITAL,
        RED,
        BLUE,
        ROCK,
        PAPER,
        SCISSOR,
        EMPTY,
        __INVALID
    };

    static constexpr Type s_selectableTypes[5] = {
        Type::RED,
        Type::BLUE,
        Type::ROCK,
        Type::PAPER,
        Type::SCISSOR
    };

    explicit Cube();

    Type type;
    bool isClicked;

    void drawCube(const std::uint8_t&& cubeStencilId);
};

}