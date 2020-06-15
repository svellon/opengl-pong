#include <GLFW/glfw3.h>

#include "polygon.h"

class Game {
public:
    Game(GLFWwindow * window);
    ~Game();

    void initGame(int screenWidth, int screenHeight, bool hardAI);
    void run();

    void setWindowSize(int screenWidth, int screenHeight);
   
private:
    GLFWwindow * window_;

    Polygon * rightPaddle_;
    Polygon * leftPaddle_;
    Polygon * ball_;

    int screenWidth_, screenHeight_;

    // Position and velocity of ball.
    // Position is redundantly stored in the polygon's private fields, but
    // we keep the positions in the game class to handle collision.
    float ballXVel_, ballYVel_;

    // score
    int player1Score_, player2Score_;

    bool hardAI_;
    bool shouldExit_;

    void render();
    void processInput();
    void updateBall();
    void updateLeftPaddle();
    void increaseBallSpeed();

    void movePaddleDown(Polygon* paddle);
    void movePaddleUp(Polygon* paddle);
   
};