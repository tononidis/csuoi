# Assignment 1

In this assignment we were tasked to create a candy-crush like game using the opengl library.

The game is launches a window where cubes arranged in a grid pattern of 15x15 are displayed.

When the user left clicks to show the menu to start a new game, the cubes change randomly to 5 types.

1 - red
2 - blue
3 - rock
4 - paper
5 - scissor

When 3 or more of these cubes align either vertically or horizontally they get destroyed and the user get score points.

If the cube is either rock/paper/scissor then the neighboring cubes around the three destroyed cubes are also destroyed based on the rules of the classic hand gestured game of rock-paper-scissor.

Neighboring cubes are categorized by 3 levels based on the distance from the cube.
Level 1: is for cubes that have a distance if 1 cube.
Level 2: is for cubes that have a distance if 2 cubes.
Level 3: is for cubes that have a distance if 3 cubes.
The diagonal cube neighbors distance is the same as the horizontal or vertical neighbors.

For an explosion of either rock/paper/scissor.
All level 1 neighbors are destroyed except its weakness based on the classic hand gestured game.
At level 2 and 3 only the cubes it beats are destroyed.

E.g If the three cubes are textured as rock.
All level 1 neighbors are destroyed except paper.
At level 2 and 3 only cubes that are of type scissor are destroyed.

How to calculate score:
+10 for the initial explosion (assignment does not state different score for explosion of 4 cubes or more, so do +10 for them as well)
+2 for each level 1 exploded cube.
-1 for each missed level 1 exploded cube.
+3 for each level 2 and 3 exploded cube.
+2 for each "reloaded" cube. (Will not be implemented due to the statement bellow)

Due to contradiction* on the assignment requirements on how to handle the destroyed cube slots. I simply do a soft reset of the board so no secondary explosions are triggered.

\* - The assignment states that "the user can move cubes at the location of the destroyed cubes", however this is contradicted by stating that "the score is updated by +2 for each cube that is reloaded". So either the cubes are reloaded automatically or the user can move cubes at the empty spots.

Allowing secondary explosions to happen due to the enormous explosion size a special cube types do, it can cascade to infinite explosions.
The cube type assignment is left to a random number generator as the assignment states. For this game to work properly the special cube types should be generated less frequent.

The game ends after 20 moves and the final score is displayed.