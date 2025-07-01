//
// Created by ERIK RAMOS on 30/06/2025.
//

#ifndef SPHERE_H
#define SPHERE_H
class Sphere {
public:
    Sphere(float x, float y, float z, float radio = 1.0f);
    void init();
    void draw(float rotX, float rotY, float r, float g, float b);
    void cleanup();

private:
    float x, y, z, radio;
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
};
#endif //SPHERE_H
