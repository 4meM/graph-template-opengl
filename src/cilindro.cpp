//
// Created by ERIK RAMOS on 1/07/2025.
//

#include "include/cilindro.h"
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
extern float cameraZ;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static std::string readFile(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Cilindro::Cilindro(float x1, float y1, float z1, float x2, float y2, float z2, float radio)
    : x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), radio(radio) {}

void Cilindro::init() {
    const int SEGMENTS = 24;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 p1(x1, y1, z1), p2(x2, y2, z2);
    glm::vec3 axis = glm::normalize(p2 - p1);
    glm::vec3 up = glm::vec3(0, 1, 0);
    if (fabs(glm::dot(axis, up)) > 0.99f) up = glm::vec3(1, 0, 0);
    glm::vec3 ortho1 = glm::normalize(glm::cross(axis, up));
    glm::vec3 ortho2 = glm::normalize(glm::cross(axis, ortho1));

    for (int i = 0; i <= SEGMENTS; ++i) {
        float theta = 2.0f * M_PI * i / SEGMENTS;
        glm::vec3 offset = radio * (float(cos(theta)) * ortho1 + float(sin(theta)) * ortho2);
        glm::vec3 v1 = p1 + offset;
        glm::vec3 v2 = p2 + offset;
        vertices.insert(vertices.end(), { v1.x, v1.y, v1.z });
        vertices.insert(vertices.end(), { v2.x, v2.y, v2.z });
    }
    for (int i = 0; i < SEGMENTS; ++i) {
        int a = i * 2, b = a + 1, c = ((i + 1) % (SEGMENTS + 1)) * 2, d = c + 1;
        indices.insert(indices.end(), { a, b, c, b, d, c });
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

void Cilindro::draw(float rotX, float rotY, float r, float g, float b) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, cameraZ));
    view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f/600.f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), r, g, b);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"), -0.5f, -1.0f, -0.3f);

    glDrawElements(GL_TRIANGLES, 24 * 6, GL_UNSIGNED_INT, 0);
}

void Cilindro::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}
