#include <glad/glad.h>

#include "game.h"
#include "shader.h"
#include "constants.h"

Game::Game(GLFWwindow * window)
{
    window_ = window;
    shouldExit_ = false;

    glfwGetWindowSize(window_, &screenWidth_, &screenHeight_);

    ballXVel_ = STARTING_BALL_VEL; ballYVel_ = STARTING_BALL_VEL;
}

Game::~Game()
{
    delete rightPaddle_;
    delete leftPaddle_;
    delete ball_;
    return;
}

void Game::initGame(int screenWidth, int screenHeight, bool hardAI)
{
    hardAI_ = hardAI;

    this->setWindowSize(screenWidth, screenHeight);

    Shader * myShader = new Shader(VS_PATH, FS_PATH);
    myShader->use();
    myShader->setVec3("scale", glm::vec3(1.0f, 1.0f, 1.0f));
    myShader->setVec3("translate", glm::vec3(0.0f, 0.0f, 0.0f));

    rightPaddle_ = new Polygon(myShader);
    rightPaddle_->setVertices(PADDLE_VERTICES, 3 * 4, RECT_INDICES, 6);
    rightPaddle_->setColor(1.0f, 0.0f, 0.0f);
    rightPaddle_->setTranslate(screenWidth_ - 50, screenHeight_ / 2);

    leftPaddle_ = new Polygon(myShader);
    leftPaddle_->setVertices(PADDLE_VERTICES, 3 * 4, RECT_INDICES, 6);
    leftPaddle_->setColor(1.0f, 0.0f, 0.0f);
    leftPaddle_->setTranslate(screenWidth_ - 50, 0.0f);
    leftPaddle_->setTranslate(50.0f, screenHeight_ / 2);

    ball_ = new Polygon(myShader);
    ball_->setColor(0.0f, 1.0f, 0.0f);
    ball_->setVertices(BALL_VERTICES, 3 * 4, RECT_INDICES, 6);
    ball_->setTranslate(screenHeight_ / 2, screenHeight_ / 2);

    player1Score_ = 0; player2Score_ = 0;
}

void Game::run()
{
    while(!shouldExit_)
    {
        this->processInput();
        this->updateBall();
        this->updateLeftPaddle();
        this->render();
        glfwPollEvents();
    }
}

void Game::setWindowSize(int width, int height)
{
    screenWidth_ = width;
    screenHeight_ = height;
    Polygon::setScreenDimensions(width, height);
} 

void Game::render()
{
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    rightPaddle_->draw();
    leftPaddle_->draw();
    ball_->draw();

    glfwSwapBuffers(window_);
    
}

void Game::updateBall()
{
    ball_->shiftTranslate(ballXVel_, ballYVel_);

    // resolve y-axis bounces against the walls
    auto ballPos = ball_->getTranslate();
    
    // collision against bottom
    if (ballPos.y - BALL_RADIUS <= 0) {
        ballYVel_ = -ballYVel_;
        increaseBallSpeed();

        // We also add the spillover to the ball's y position.
        // If the ball's bottom would be 3 pixels below the screen this frame
        // it gets set to 3 pixels above the bottom of the screen since it bounced
        // and moves up a bit.
        float spillover = -(ballPos.y - BALL_RADIUS);
        ball_->setTranslate(ballPos.x, 0 + spillover + BALL_RADIUS);
    }

    // collision against top
    if (ballPos.y + BALL_RADIUS >= screenHeight_) {
        ballYVel_ = -ballYVel_;
        increaseBallSpeed();
        float spillover = -(screenHeight_ - (ballPos.y + BALL_RADIUS));
        ball_->setTranslate(ballPos.x, screenHeight_ - spillover - BALL_RADIUS);
    }


    // Collisions against walls reset the ball's speed and move it back to
    // the screen center.
    // collision against left wall
    if (ballPos.x < 0) {
        ball_->setTranslate(screenWidth_/2, screenHeight_/2);
        ballXVel_ = STARTING_BALL_VEL; ballYVel_ = STARTING_BALL_VEL;
    }
    // collision against right wall
    if (ballPos.x > screenWidth_) {
        ball_->setTranslate(screenWidth_/2, screenHeight_/2);
        ballXVel_ = STARTING_BALL_VEL; ballYVel_ = STARTING_BALL_VEL;
    }

    // TODO account for spillover in paddle collision logic

    // collision against leftPaddle
    auto paddlePos = leftPaddle_->getTranslate();
    if (
        ballPos.x > paddlePos.x - (PADDLE_WIDTH / 2) - BALL_RADIUS &&
        ballPos.x < paddlePos.x + (PADDLE_WIDTH / 2) + BALL_RADIUS &&
        ballPos.y > paddlePos.y - (PADDLE_HEIGHT / 2) - BALL_RADIUS &&
        ballPos.y < paddlePos.y + (PADDLE_HEIGHT / 2) + BALL_RADIUS
    ) {
        if (ballXVel_ < 0) ballXVel_ = -ballXVel_;
        increaseBallSpeed();
    }

    // collision against rightPaddle
    paddlePos = rightPaddle_->getTranslate();
    if (
        ballPos.x > paddlePos.x - (PADDLE_WIDTH / 2) - BALL_RADIUS &&
        ballPos.x < paddlePos.x + (PADDLE_WIDTH / 2) + BALL_RADIUS &&
        ballPos.y > paddlePos.y - (PADDLE_HEIGHT / 2) - BALL_RADIUS &&
        ballPos.y < paddlePos.y + (PADDLE_HEIGHT / 2) + BALL_RADIUS
    ) {
        if (ballXVel_ > 0) ballXVel_ = -ballXVel_;
        increaseBallSpeed();
    }

}

/** Updates the left (AI) paddle
 * AI logic is simple: if the ball's center is below the paddle's center
 * move the paddle down. If the ball's center is above the paddle's center
 * move the paddle up. */
void Game::updateLeftPaddle()
{
    auto ballPos = ball_->getTranslate();
    auto paddlePos = leftPaddle_->getTranslate();

    /* Hard AI logic:
     * If the ball is moving away from the left paddle, move to center.
     * If the ball is moving towards the left paddle:
     *  1. Compute the intersection of the ball with the left paddle's x
     *     position using the ball's x and y velocity.
     *  2. Move the paddle to the y value of the intersection.
     */
    if (hardAI_) {
        if (ballXVel_ > 0) {
            // pad right hand by paddle velocity to prevent the paddle from "vibrating"
            if (paddlePos.y > screenHeight_ / 2 + PADDLE_VEL)
                movePaddleDown(leftPaddle_);
            else if (paddlePos.y < screenHeight_ / 2 - PADDLE_VEL)
                movePaddleUp(leftPaddle_);
        } else {
            // xDiff is total x-distance ball will travel
            float xDiff = ballPos.x - paddlePos.x;
            float slope = ballYVel_ / std::abs(ballXVel_);
            float intersect = ballPos.y + (xDiff * slope);

            // The "intersect" can be negative or greater than the screen height.
            // In that case we need to compute reflections across the top or bottom wall
            // to obtain the true intsersection that will occur after bouncing.
            while (intersect > screenHeight_ || intersect < 0)
            {
                // If we have a raw y intersect of -50, it'll bounce off the bottom wall
                // and become positive 50.
                if (intersect < 0)
                    intersect = -intersect;
                
                // if the intersect is screen height + 50, then the true intersect
                // bill become screen height - 50 after bouncing over the top wall.
                if (intersect > screenHeight_)
                    intersect = screenHeight_ - (intersect - screenHeight_);
            }

            // TODO maybe make a helper to avoid near-dupe code
            if (paddlePos.y > intersect + PADDLE_VEL)
                movePaddleDown(leftPaddle_);
            else if (paddlePos.y < intersect - PADDLE_VEL)
                movePaddleUp(leftPaddle_);

        }
    } else {
        if (ballPos.y > paddlePos.y)
            movePaddleUp(leftPaddle_);
        if (ballPos.y < paddlePos.y)
            movePaddleDown(leftPaddle_);
    }
}

// randomly increases either the vertical or horizontal ball speed
void Game::increaseBallSpeed()
{
    if (std::rand() %2) {
        // make sure to add negative velocity is current velocity is negative
        ballXVel_ += (ballXVel_ > 0) ? BALL_BOUNCE_D_VEL : -BALL_BOUNCE_D_VEL;
    } else {
        ballYVel_ += (ballYVel_ > 0) ? BALL_BOUNCE_D_VEL : -BALL_BOUNCE_D_VEL;
    }
}

void Game::processInput()
{
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        shouldExit_ = true;
    if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS) {
        movePaddleUp(rightPaddle_);
    }
    if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
        movePaddleDown(rightPaddle_);
    }
}

void Game::movePaddleUp(Polygon* paddle)
{
    paddle->shiftTranslate(0.0f, PADDLE_VEL);
    // don't go above the screen
    auto paddlePos = paddle->getTranslate();
    if (paddlePos.y + PADDLE_HEIGHT / 2 > screenHeight_) {
        paddle->setTranslate(paddlePos.x, screenHeight_ - PADDLE_HEIGHT / 2);
    }
}

void Game::movePaddleDown(Polygon* paddle)
{
    paddle->shiftTranslate(0.0f, -PADDLE_VEL);
    // don't go below the screen
    auto paddlePos = paddle->getTranslate();
    if (paddlePos.y - PADDLE_HEIGHT / 2 < 0) {
        paddle->setTranslate(paddlePos.x, PADDLE_HEIGHT / 2);
    }
}
