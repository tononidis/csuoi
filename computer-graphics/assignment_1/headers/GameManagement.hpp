#pragma once

#include "Config.hpp"
#include "Cube.hpp"
#include "Utils.hpp"

#include <cstdint>

namespace BraXaPsaIII {

namespace GameManagement {

    enum class GameState {
        DEFAULT,
        NEW_GAME,
        GAME_OVER,
        END_GAME
    };

    inline std::uint32_t count = 0;
    inline std::uint32_t score = 0;

    inline array2d<Cube, Config::GRID_SIZE, Config::GRID_SIZE> cubes2dArr;
    inline GameState gameState = GameState::DEFAULT;

    void createNewGame(void);
    void checkPointAndDestroyCubes(const Utils::Point& point);

}

}