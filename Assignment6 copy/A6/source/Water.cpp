#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Water.h"
#include "ImageData.h"
#endif
// plane with an origin at [min, 0, min]
void planeMeshQuads(std::vector<Water::Vertex> &verts, std::vector<unsigned int> &indices,
    float min, float max, float stepsize) {
    float x = min;
    float y = 0;

    for (float x = min; x <= max; x += stepsize) {
        for (float z = min; z <= max; z += stepsize) {

            // Linearly map [min, max] -> [0,1] for UV coordinates
            float u = (x - min) / (max - min);
            float v = (z - min) / (max - min);

            // Plane origin is (min, 0, min) putting in v coordinate so scene faces us correctly
            v = 1 - v;

            Water::Vertex vert;
            vert.position = glm::vec3(x, y, z);
            vert.uv = glm::vec2(u, v);
            vert.normal = glm::vec3(0, 1, 0);
            verts.emplace_back(vert);
        }
    }

    // Index data
    int colSize = (max - min) / stepsize + 1;
    int i = 0, j = 0;
    for (float x = min; x <= max - 1; x += stepsize) {
        j = 0;
        for (float z = min; z <= max - 1; z += stepsize) {
            
            // Patch indices
            indices.emplace_back(i * colSize + j);
            indices.emplace_back(i * colSize + j + 1);
            indices.emplace_back((i + 1) * colSize + j + 1);
            indices.emplace_back((i + 1) * colSize + j);
            j++;
        }
        i++;
    }
}

Water::Water(float min, float max, float stepsize): VAO(0), VBO(0), EBO(0) {
    // Generating plane into vertex and index vectors
    planeMeshQuads(verts, indices, min, max, stepsize);

    // Generate VAO & VBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

    // Pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);

    // Norm
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Getting texture data for water
    ImageData imgData("/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/assets/water.bmp");
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgData.width, imgData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    imgData.free();

    // Displacement map texture
    ImageData dmData("/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/assets/displacement-map1.bmp");
    glGenTextures(1, &dispMapTexture);
    glBindTexture(GL_TEXTURE_2D, dispMapTexture);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dmData.width, dmData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, dmData.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    dmData.free();
    
    // Boat data
    ImageData boatData("/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/assets/boat1.bmp");
    glGenTextures(1, &boatTexture);
    glBindTexture(GL_TEXTURE_3D, boatTexture);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, boatData.width, boatData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, boatData.data);
    glGenerateMipmap(GL_TEXTURE_3D);
    boatData.free();

    // head data
     ImageData headData("/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/assets/head1.bmp");
    glGenTextures(1, &headTexture);
    glBindTexture(GL_TEXTURE_2D, headTexture);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, headData.width, headData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, headData.data);
    glGenerateMipmap(GL_TEXTURE_3D);
    headData.free();
    
    // eyes data
     ImageData eyesData("/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/assets/eyes1.bmp");
    glGenTextures(1, &eyesTexture);
    glBindTexture(GL_TEXTURE_2D, eyesTexture);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, eyesData.width, eyesData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, eyesData.data);
    glGenerateMipmap(GL_TEXTURE_3D);
    eyesData.free();

}

void Water::draw(ShaderProgram *shader, glm::mat4 view, glm::mat4 projection) {
    glBindVertexArray(VAO);
    glUseProgram(shader->ID);

    shader->setUniformMatrix4fv("mvp", projection * view);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->setUniform1i("tex", 0);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dispMapTexture);
    shader->setUniform1i("dispMap", 1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, boatTexture);
    shader->setUniform1i("boat", 2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_3D, headTexture);
    shader->setUniform1i("head", 3);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_3D, eyesTexture);
    shader->setUniform1i("eyes", 4);

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}
