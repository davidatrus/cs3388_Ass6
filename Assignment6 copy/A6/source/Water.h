
#ifndef WATER_H
#define WATER_H

#include <glm/mat4x4.hpp>
#include <vector>
#include "ShaderProgram.h"

class Water {
public:
    struct Vertex {
        glm::vec3 position; 
        glm::vec2 uv;
        glm::vec3 normal;
    };

private:
    GLuint texture, dispMapTexture,boatTexture,headTexture, eyesTexture, VAO, VBO, EBO;
    std::vector<Vertex> verts;
    std::vector<unsigned int> indices;

public:
    Water(float min, float max, float stepsize);
    void draw(ShaderProgram* shader, glm::mat4 view, glm::mat4 projection);
};

#endif
