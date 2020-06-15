#include <vector>

#include "shader.h"

class Polygon {
public:
    Polygon(Shader *);
    ~Polygon();

    void setVertices(float *, int, unsigned int *, int);
    void setVertices(std::vector<float>&, std::vector<unsigned int>&);

    // Property setters. These directly substitute Polygon properties
    void setColor(float, float, float, float=1.0f);
    void setTranslate(float, float, float=0.0f);
    void setRotateDegrees(float);

    glm::vec3 getTranslate();

    // Scale is used to keep polygon dimensions and position consistent
    // regardless of screen dimensions
    void setScale(float, float);

    static void setScreenDimensions(int, int);

    // Propery shift methods add to the propety. E.g.
    // shiftTranslate(1.0, 0.0) shifts the polygon to the right by 1.0
    void shiftTranslate(float, float, float=0.0f);
    void shiftRotateDegrees(float);

    // Bind the polygon's vertex array and draw.
    void draw();

    float * getVertices();
    int getNumVertices();
private:
    static glm::ivec2 screenDimensions_;

    void deleteBuffers();

    Shader * shader_;
   
    glm::vec2 scale_;
    glm::vec3 translate_;
    glm::vec4 color_;

    float rotateDegrees_;

    int numVertices_, numIndices_;

    unsigned int VAO_, VBO_, EBO_;
};