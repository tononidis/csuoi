#pragma once

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace BraXaPsaIII {

namespace TexturesInfo {

inline GLuint rock;
inline GLuint paper;
inline GLuint scissor;

void loadTextures();
} // namespace TexturesInfo

} // namespace BraXaPsaIII