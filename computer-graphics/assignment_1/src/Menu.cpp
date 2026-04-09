#include "Menu.hpp"
#include "GameManagement.hpp"

#include <GL/glut.h>

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
            SPDLOG_INFO("Game Started!");

            GameManagement::createNewGame();
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
}

}