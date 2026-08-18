#include "Menu.hpp"

#include "GameManagement.hpp"

#ifdef _WIN32
#include <windows.h>
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

#include <spdlog/spdlog.h>

namespace BraXaPsaIII {

namespace Menu {
static void menuEventHandler(int value)
{
    if (static_cast<GameManagement::GameState>(value) == GameManagement::GameState::END_GAME) {
        GameManagement::gameState = GameManagement::GameState::END_GAME;
        SPDLOG_INFO("Ending the game ...");
        std::exit(EXIT_SUCCESS);
    }

    else if (static_cast<GameManagement::GameState>(value) == GameManagement::GameState::NEW_GAME) {
        GameManagement::gameState = GameManagement::GameState::NEW_GAME;

        SPDLOG_DEBUG("Stating the game ...");
        GameManagement::createNewGame();
        SPDLOG_INFO("Game Started!");
    }

    else {
        SPDLOG_CRITICAL("Unknown menu event!");
        std::exit(EXIT_FAILURE);
    }
}

void registerMenu()
{
    glutCreateMenu(menuEventHandler);

    glutAddMenuEntry("Start Game", static_cast<int>(GameManagement::GameState::NEW_GAME));
    glutAddMenuEntry("Exit", static_cast<int>(GameManagement::GameState::END_GAME));
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
} // namespace Menu

} // namespace BraXaPsaIII