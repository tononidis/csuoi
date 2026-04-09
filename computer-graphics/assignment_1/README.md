# Assignment 1

In this assignment we were tasked to create a candy-crush like game using the opengl library.

The game is launches a window where cubes arranged in a grid pattern of 15x15 are displayed.

When the user left clicks to show the menu to start a new game, the cubes change randomly to 5 types.

1 - red
2 - blue
3 - rock
4 - paper
5 - scissor

When 3 of these cubes align either vertically or horizontally they get destroyed and the user get score points.

If the cube is either rock/paper/scissor then the neighboring cubes around the three destroyed cubes are also destroyed based on the rules of the classic hand gestured game of rock-paper-scissor.

E.g If the three cubes are textured as rock then the neighboring cubes that are textured as scissor are destroyed as well.

The game ends after 20 moves and the final score is displayed.