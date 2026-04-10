#pragma once

#include <array>

namespace BraXaPsaIII {

template <typename T, std::size_t N, std::size_t M>
using array2d = std::array<std::array<T, N>, M>;

namespace Config {
    inline constexpr auto GRID_SIZE = 15;
    inline constexpr auto START_HEIGHT = 600;
    inline constexpr auto START_WIDTH = 600;
    inline constexpr auto WINDOW_START_POS_X = 100;
    inline constexpr auto WINDOW_START_POS_Y = 100;
    inline constexpr auto WINDOW_NAME = "BraXaPsaIII";
};

static_assert(Config::GRID_SIZE > 10, "Grid size must be larger than 10");

}