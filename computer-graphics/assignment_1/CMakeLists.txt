cmake_minimum_required(VERSION 4.1)

project(
    BraXaPsaIII
    VERSION 0.1
    DESCRIPTION "A candycrash-like game implemented in C++ using OpenGL and GLFW."
    LANGUAGES CXX
)
    
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-linux)

find_path(STB_INCLUDE_DIRS "stb_image.h")
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_compile_options(-g)

add_compile_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)

add_executable(
    ${PROJECT_NAME}
    "src/BraXaPsaIII.cpp"
    "src/WindowUtils.cpp"
    "src/TexturesInfo.cpp"
    "src/Cube.cpp"
    "src/Display.cpp"
    "src/Effects.cpp"
    "src/GameManagement.cpp"
    "src/Menu.cpp"
    "src/Mouse.cpp"
    "src/Keyboard.cpp"
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    X11
    spdlog::spdlog
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    headers
    ${STB_INCLUDE_DIRS}
)