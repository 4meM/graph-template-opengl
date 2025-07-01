//
// Created by ERIK RAMOS on 1/07/2025.
//

#ifndef CILINDRO_H
#define CILINDRO_H
class Cilindro {
public:
    Cilindro(float x1, float y1, float z1, float x2, float y2, float z2, float radio = 0.1f);
    void init();
    void draw(float rotX, float rotY, float r, float g, float b);
    void cleanup();
private:
    float x1, y1, z1, x2, y2, z2, radio;
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
};
#endif //CILINDRO_H
