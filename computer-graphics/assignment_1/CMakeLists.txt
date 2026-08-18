cmake_minimum_required(VERSION 4.1)

project(
    BraXaPsaIII
    VERSION 0.1
    DESCRIPTION "A candycrash-like game implemented in C++ using OpenGL and GLFW."
    LANGUAGES CXX
)

find_path(STB_INCLUDE_DIRS "stb_image.h")
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(ASSET_DIR "${CMAKE_SOURCE_DIR}/images")

if(UNIX)
add_compile_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
add_compile_definitions(-DASSET_DIR="${ASSET_DIR}")
endif()

if(MSVC)
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
add_compile_definitions(ASSET_DIR="${ASSET_DIR}")
endif()


add_executable(
    ${PROJECT_NAME}
    "src/BraXaPsaIII.cpp"
    "src/Cube.cpp"
    "src/Display.cpp"
    "src/Effects.cpp"
    "src/GameManagement.cpp"
    "src/Keyboard.cpp"
    "src/Logic.cpp"
    "src/Menu.cpp"
    "src/Mouse.cpp"
    "src/Point.cpp"
    "src/TexturesInfo.cpp"
    "src/WindowUtils.cpp"
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    spdlog::spdlog
)

if(UNIX AND NOT APPLE)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE X11
)
endif()

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    headers
    ${STB_INCLUDE_DIRS}
)