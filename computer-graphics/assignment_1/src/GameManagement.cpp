#include "GameManagement.hpp"

#include <GL/glut.h>

#include <cstdlib>

namespace BraXaPsaIII {

namespace GameManagement {
    void destroyCubes(void)
    {
        for (auto row = 0; row < GameManagement::cubes2dArr.size(); row++) {
            for (auto column = 0; column < GameManagement::cubes2dArr[0].size(); column++) {
                if (row < GameManagement::cubes2dArr.size() - 2) {
                    auto firstRowState = GameManagement::cubes2dArr[row][column].type;
                    auto secondRowState = GameManagement::cubes2dArr[row + 1][column].type;
                    auto thirdRowState = GameManagement::cubes2dArr[row + 2][column].type;

                    if (firstRowState == secondRowState && secondRowState == thirdRowState && firstRowState != Cube::Type::EMPTY) {
                        GameManagement::cubes2dArr[row][column].type = Cube::Type::EMPTY;
                        GameManagement::cubes2dArr[row + 1][column].type = Cube::Type::EMPTY;
                        GameManagement::cubes2dArr[row + 2][column].type = Cube::Type::EMPTY;

                        glutPostRedisplay();
                    }
                }

                if (column < GameManagement::cubes2dArr[0].size() - 2) {
                    auto firstColumnState = GameManagement::cubes2dArr[row][column].type;
                    auto secondColumnState = GameManagement::cubes2dArr[row][column + 1].type;
                    auto thirdColumnState = GameManagement::cubes2dArr[row][column + 2].type;

                    if (firstColumnState == secondColumnState && secondColumnState == thirdColumnState && firstColumnState != Cube::Type::EMPTY) {
                        GameManagement::cubes2dArr[row][column].type = Cube::Type::EMPTY;
                        GameManagement::cubes2dArr[row][column + 1].type = Cube::Type::EMPTY;
                        GameManagement::cubes2dArr[row][column + 2].type = Cube::Type::EMPTY;

                        glutPostRedisplay();
                    }
                }
            }
        }
    }

    void createNewGame(void)
    {
        int randomNum = -1;
        for (auto row = 0; row < cubes2dArr.size(); row++) {
            for (auto column = 0; column < cubes2dArr[0].size(); column++) {
                randomNum = std::rand() % std::size(Cube::s_selectableTypes);
                cubes2dArr[row][column].type = Cube::s_selectableTypes[randomNum];
            }
        }

        glutPostRedisplay();
    }
}

};