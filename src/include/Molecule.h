//
// Created by ERIK RAMOS on 1/07/2025.
//

#ifndef MOLECULE_H
#define MOLECULE_H
#include <vector>
#include <string>
#include "sphere.h"
#include "cilindro.h"

struct Atom {
    float x, y, z;
    std::string type;
};

class Molecule {
public:
    Molecule(const std::string& sdfPath);
    void init();
    void draw(float rotX, float rotY);
    void cleanup();
private:
    std::vector<Atom> atoms;
    std::vector<Sphere> spheres;
    std::vector<Cilindro> cilindros;
    void loadSDF(const std::string& path);
    void atomColor(const std::string& type, float& r, float& g, float& b);
};
#endif //MOLECULE_H
