Implementation of Pong with OpenGL and GLFW. Built on Mac OSX.
`brew install glfw glm` should fetch all required dependencies. Make using `make` or `make debug`.
Execute using `./bin/start` or `./bin/start -hard_ai` (you need to be project root directory, since relative paths are used).
Default AI just moves paddle to the ball's current position. Hard AI computes predicted intersection of the ball with the paddle's x coordinate and moves there. Hard AI is basically unbeatable.

`main.cpp` contains the GLFW setup code and argument parsing.
The `Polygon` class contains abstractions over OpenGL to create a polygons using vertices and indices, and exposes several functions to move, rotate, and scale the polgon as well as a `draw()` function to draw the polygon.
The `Game` class contains the game logic. Collision is handled in `updateBall()`. human controlled paddle is controlled in the `processInput()` and `movePaddle()` methods. The AI controlled paddle is controlled by the `updateLeftPaddle()` method.

The vertex shader applies the transformations specified in the Polygon class, as well as screen scaling. Origin is bottom left of the screen. The fragment shader just applies a single color specified by the Polygon class.

Would not recommend using this as a template for your own OpenGL projects. Several jank items are present, namely the use of separate uniform variables to control translation, scaling, and rotation. Also the use of rendundant vertex and element buffers for the left and right paddles despite them being the same polygon.
