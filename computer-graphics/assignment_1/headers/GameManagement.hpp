#pragma once

#include "Config.hpp"
#include "Cube.hpp"
#include "Point.hpp"

#include <cstdint>
#include <vector>

namespace BraXaPsaIII {

namespace GameManagement {

enum class GameState { DEFAULT, NEW_GAME, EFFECT_IS_APPLIED, GAME_OVER, END_GAME };

#ifndef NUMBER_OF_MOVES
#define NUMBER_OF_MOVES 20
#endif

inline auto numberOfMovesLeft = NUMBER_OF_MOVES;
inline std::uint32_t score = 0;

inline array2d<Cube, Config::GRID_SIZE, Config::GRID_SIZE> cubes2dArr;
inline GameState gameState = GameState::DEFAULT;

Cube::Type generateRandomSelectableType();
Cube::Type generateRandomSelectableTypeExclude(Cube::Type excludedType);
bool getDestructibleCubePoints(std::vector<Point> &destructiblePoints_out, bool isUsedForReplacement);
void replaceEmptyCubesWithOutScoring(void);
void createNewGame(void);

} // namespace GameManagement

} // namespace BraXaPsaIII