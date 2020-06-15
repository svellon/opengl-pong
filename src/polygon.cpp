#include <glad/glad.h>

#include "polygon.h"

// Static variable definitions
glm::ivec2 Polygon::screenDimensions_;


Polygon::Polygon(Shader * shader)
{
    this->shader_ = shader;
    
    // default, no scale or translation
    this->setScale(1.0f, 1.0f);
    this->setTranslate(0.0f, 0.0f, 0.0f);
    this->rotateDegrees_ = 0.0f;
}

void Polygon::setVertices(float * vertices, int numVertices, unsigned int * indices, int numIndices)
{
    this->numVertices_ = numVertices;
    this->numIndices_ = numIndices;
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Polygon::setVertices(std::vector<float> &vertexVec, std::vector<unsigned int> &indexVec)
{
    float * vertices = (float*) malloc(sizeof(float) * vertexVec.size());
    unsigned int * indices = (unsigned int *) malloc(sizeof(unsigned int) * indexVec.size());
    this->setVertices(vertices, vertexVec.size(), indices, indexVec.size());
}

Polygon::~Polygon()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
}

void Polygon::setScale(float scaleX, float scaleY)
{
    this->scale_ = glm::vec2(scaleX, scaleY);
}

void Polygon::setScreenDimensions(int width, int height)
{
    screenDimensions_ = glm::ivec2(width, height);
}

void Polygon::setTranslate(float transX, float transY, float transZ)
{
    this->translate_ = glm::vec3(transX, transY, transZ);
}

glm::vec3 Polygon::getTranslate()
{
    return this->translate_;
}

void Polygon::setRotateDegrees(float newRotation)
{
    this->rotateDegrees_ = newRotation;
}

void Polygon::setColor(float r, float g, float b, float a)
{
    this->color_ = glm::vec4(r, g, b, a);
}

void Polygon::shiftTranslate(float dx, float dy, float dz)
{
    this->translate_ = glm::vec3(
        this->translate_.x + dx,
        this->translate_.y + dy,
        this->translate_.z + dz
    );
}
void Polygon::shiftRotateDegrees(float dDegrees)
{
    this->rotateDegrees_ += dDegrees;
}

void Polygon::deleteBuffers()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
}

void Polygon::draw()
{
    this->shader_->setVec4("ourColor", this->color_);
    this->shader_->setIVec2("screenDimensions", screenDimensions_);
    this->shader_->setVec2("scale", this->scale_);
    this->shader_->setVec3("translate", this->translate_);
    this->shader_->setFloat("rotateDegrees", this->rotateDegrees_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glDrawElements(GL_TRIANGLES, this->numIndices_, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}