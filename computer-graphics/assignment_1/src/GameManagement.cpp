#include "GameManagement.hpp"

#include <cstdlib>

namespace BraXaPsaIII {

namespace GameManagement {
    void createNewGame(void)
    {
        int randomNum = -1;
        for (auto row = 0; row < cubes2dArr.size(); row++) {
            for (auto column = 0; column < cubes2dArr[0].size(); column++) {
                randomNum = std::rand() % 5;

                if (randomNum == 0) {
                    cubes2dArr[row][column].type = Cube::Type::RED;
                }

                else if (randomNum == 1) {
                    cubes2dArr[row][column].type = Cube::Type::BLUE;
                }

                else if (randomNum == 2) {
                    cubes2dArr[row][column].type = Cube::Type::ROCK;
                }

                else if (randomNum == 3) {
                    cubes2dArr[row][column].type = Cube::Type::PAPER;
                }

                else if (randomNum == 4) {
                    cubes2dArr[row][column].type = Cube::Type::SCISSOR;
                }
            }
        }
    }
}

};