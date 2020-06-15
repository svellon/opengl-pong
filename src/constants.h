// ball is actually a square. Radius is 1/2 width & height
inline float BALL_RADIUS = 15.0f;

inline float STARTING_BALL_VEL = -5.0f;
inline float BALL_BOUNCE_D_VEL = 0.25f;

inline float PADDLE_VEL = 5.0f;

inline float PADDLE_HEIGHT = 250.0f;
inline float PADDLE_WIDTH = 40.0f;

inline float PADDLE_VERTICES[] = {
    -PADDLE_WIDTH/2, -PADDLE_HEIGHT/2, 0.0f,
    -PADDLE_WIDTH/2, PADDLE_HEIGHT/2, 0.0f,
    PADDLE_WIDTH/2, PADDLE_HEIGHT/2, 0.0f,
    PADDLE_WIDTH/2, -PADDLE_HEIGHT/2, 0.0f
};
inline float BALL_VERTICES[] = {
    -BALL_RADIUS, -BALL_RADIUS, 0.0f,
    -BALL_RADIUS, BALL_RADIUS, 0.0f,
    BALL_RADIUS, BALL_RADIUS, 0.0f,
    BALL_RADIUS, -BALL_RADIUS, 0.0f
};
inline unsigned int RECT_INDICES[] = {0, 1, 2, 2, 3, 0};

inline const char * VS_PATH =  "./shaders/basic.vs";
inline const char * FS_PATH =  "./shaders/basic.fs";

inline const char * AI_ARG = "-hard_ai";