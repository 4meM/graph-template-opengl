//
// Created by ERIK RAMOS on 30/06/2025.
//

#include "include/sphere.h"
#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
extern float cameraZ;
const int STACKS = 20;
const int SLICES = 20;

static std::string readFile(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Sphere::Sphere(float x, float y, float z, float radio) : x(x), y(y), z(z), radio(radio) {}

void Sphere::init() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= STACKS; ++i) {
        float phi = i * M_PI / STACKS;
        for (int j = 0; j <= SLICES; ++j) {
            float theta = j * 2 * M_PI / SLICES;
            float xpos = radio * sin(phi) * cos(theta); // Usa radio
            float ypos = radio * cos(phi);
            float zpos = radio * sin(phi) * sin(theta);
            vertices.insert(vertices.end(), { xpos, ypos, zpos });
        }
    }

    for (int i = 0; i < STACKS; ++i) {
        for (int j = 0; j < SLICES; ++j) {
            int first = i * (SLICES + 1) + j;
            int second = first + SLICES + 1;
            indices.insert(indices.end(), {
                first, second, first + 1,
                second, second + 1, first + 1
            });
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::string vertexCode = readFile("../shaders/vertex_shader.glsl");
    std::string fragmentCode = readFile("../shaders/fragment_shader.glsl");
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Sphere::draw(float rotX, float rotY, float r, float g, float b) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, cameraZ));
    view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f/600.f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), r, g, b);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"), -0.5f, -1.0f, -0.3f);

    glDrawElements(GL_TRIANGLES, STACKS * SLICES * 6, GL_UNSIGNED_INT, 0);
}

void Sphere::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}
