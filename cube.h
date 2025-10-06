//
//  cube.h
//  test
//
//  Created by Nazirul Hasan on 4/10/23.
//

#ifndef cube_h
#define cube_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Cube {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Cube()
    {
        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Cube()
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteVertexArrays(1, &lightTexCubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &cubeEBO);
    }

    void drawCubeWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    }

    void drawCubeWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCube(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int lightTexCubeVAO;
    unsigned int cubeVBO;
    unsigned int cubeEBO;

    void setUpCubeVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float cube_vertices[] = {
            // positions      // normals         // texture
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,

            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,
            0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,

            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,

            0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmax
        };

        unsigned int cube_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenVertexArrays(1, &lightCubeVAO);
        glGenVertexArrays(1, &lightTexCubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);


        glBindVertexArray(lightTexCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};
class Prism {
public:

    // Material properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Texture properties
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // Common properties
    float shininess;

    // Constructors
    Prism() {
        setUpPrismVertexDataAndConfigureVertexAttribute();
    }

    Prism(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny) {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpPrismVertexDataAndConfigureVertexAttribute();
    }

    Prism(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax) {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpPrismVertexDataAndConfigureVertexAttribute();
    }

    // Destructor
    ~Prism() {
        glDeleteVertexArrays(1, &prismVAO);
        glDeleteBuffers(1, &prismVBO);
        glDeleteBuffers(1, &prismEBO);
    }

    void drawPrism(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();
        shader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        glBindVertexArray(prismVAO);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0); // 18 indices for a triangular prism
    }

private:
    unsigned int prismVAO;
    unsigned int prismVBO;
    unsigned int prismEBO;

    void setUpPrismVertexDataAndConfigureVertexAttribute() {
        // Define the vertex data for a triangular prism
        float prism_vertices[] = {
            // Positions          // Normals          // Texture Coords
             0.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,  TXmin, TYmin, // Bottom triangle
             1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,  TXmax, TYmin,
             0.5f, 0.5f, 0.0f,    0.0f, 0.0f, -1.0f,  (TXmin + TXmax) / 2, TYmax,

             0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   TXmin, TYmin, // Top triangle
             1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   TXmax, TYmin,
             0.5f, 0.5f, 1.0f,    0.0f, 0.0f, 1.0f,   (TXmin + TXmax) / 2, TYmax,

             // Rectangular sides
             0.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,  TXmin, TYmin, //6
             0.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,  TXmax, TYmin,
             0.5f, 0.5f, 1.0f,    -1.0f, 0.0f, 0.0f,   TXmax, TYmax,
             0.5f, 0.5f, 0.0f,    -1.0f, 0.0f, 0.0f,  TXmin, TYmax,

             1.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,  TXmin, TYmin, //10
             0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  TXmin, TYmax,
             0.5f, 0.5f, 1.0f,     1.0f, 0.0f, 0.0f,  TXmax, TYmax,
             1.0f, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f,  TXmax, TYmin,

             0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,  TXmin, TYmin, //14
             0.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f,  TXmin, TYmax,
             1.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f,  TXmax, TYmax,
             1.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,  TXmax, TYmin
        };

        unsigned int prism_indices[] = {
            // Bottom face
            0, 1, 2,
            // Top face
            3, 4, 5,
            // Side faces
            6, 9, 8,
            8, 7, 6,

            10, 13, 12,
            12, 11, 10,

            14, 17, 16,
            16, 15, 14
        };

        glGenVertexArrays(1, &prismVAO);
        glGenBuffers(1, &prismVBO);
        glGenBuffers(1, &prismEBO);

        glBindVertexArray(prismVAO);

        glBindBuffer(GL_ARRAY_BUFFER, prismVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(prism_vertices), prism_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prismEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prism_indices), prism_indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
};
class Prismforcar {
public:

    // Material properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Texture properties
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // Common properties
    float shininess;

    // Constructors
    Prismforcar() {
        setUpPrismVertexDataAndConfigureVertexAttribute();
    }

    Prismforcar(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny) {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpPrismVertexDataAndConfigureVertexAttribute();
    }

    Prismforcar(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax) {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpPrismVertexDataAndConfigureVertexAttribute();
    }

    // Destructor
    ~Prismforcar() {
        glDeleteVertexArrays(1, &prismVAO);
        glDeleteBuffers(1, &prismVBO);
        glDeleteBuffers(1, &prismEBO);
    }

    void drawPrismforcar(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();
        shader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        glBindVertexArray(prismVAO);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0); // 18 indices for a triangular prism
    }

private:
    unsigned int prismVAO;
    unsigned int prismVBO;
    unsigned int prismEBO;

    void setUpPrismVertexDataAndConfigureVertexAttribute() {
        // Define the vertex data for a triangular prism
        float prism_vertices[] = {
            // Positions          // Normals          // Texture Coords
             0.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,  1.0, 0.0, // Bottom triangle
             1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,  0.0, 0.4,
             0.5f, 0.5f, 0.0f,    0.0f, 0.0f, -1.0f,  0.6, 1.0,

             0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0, 0.0, // Top triangle
             1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0, 0.4,
             0.5f, 0.5f, 1.0f,    0.0f, 0.0f, 1.0f,   0.6, 1.0,

             // Rectangular sides
             0.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,  0.65, 0.4, //6
             0.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,  0.67, 0.4,
             0.5f, 0.5f, 1.0f,    -1.0f, 0.0f, 0.0f,   0.67, 0.46,
             0.5f, 0.5f, 0.0f,    -1.0f, 0.0f, 0.0f,  0.65, 0.46,

             1.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,  0.8, 0.8, //10
             0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  0.8, 0.9,
             0.5f, 0.5f, 1.0f,     1.0f, 0.0f, 0.0f,  0.9, 0.9,
             1.0f, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f,  0.9, 0.8,

             0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,  0.8, 0.8, //14
             0.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f,  0.8, 0.9,
             1.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f,  0.9, 0.9,
             1.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,  0.9, 0.8
        };

        unsigned int prism_indices[] = {
            // Bottom face
            0, 1, 2,
            // Top face
            3, 4, 5,
            // Side faces
            6, 9, 8,
            8, 7, 6,

            10, 13, 12,
            12, 11, 10,

            14, 17, 16,
            16, 15, 14
        };

        glGenVertexArrays(1, &prismVAO);
        glGenBuffers(1, &prismVBO);
        glGenBuffers(1, &prismEBO);

        glBindVertexArray(prismVAO);

        glBindBuffer(GL_ARRAY_BUFFER, prismVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(prism_vertices), prism_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prismEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prism_indices), prism_indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
};

class Cube_for_door {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Cube_for_door()
    {
        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube_for_door(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube_for_door(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Cube_for_door()
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteVertexArrays(1, &lightTexCubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &cubeEBO);
    }

    void drawCubeWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    }

    void drawCubeWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCube(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int lightTexCubeVAO;
    unsigned int cubeVBO;
    unsigned int cubeEBO;

    void setUpCubeVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float cube_vertices[] = {
            // positions      // normals         // texture
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,

            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,
            0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,

            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,

            0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmax
        };

        unsigned int cube_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenVertexArrays(1, &lightCubeVAO);
        glGenVertexArrays(1, &lightTexCubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);


        glBindVertexArray(lightTexCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};

#endif /* cube_h */
