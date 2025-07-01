//
// Created by ERIK RAMOS on 1/07/2025.
//

// src/Molecule.cpp
#include "include/molecule.h"
#include <fstream>
#include <sstream>
#include <iostream>

Molecule::Molecule(const std::string& sdfPath) {
    loadSDF(sdfPath);
}

void Molecule::loadSDF(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    int atomCount = 0, bondCount = 0;

    for (int i = 0; i < 3; ++i) std::getline(file, line);
    std::getline(file, line);
    std::istringstream iss(line);
    iss >> atomCount >> bondCount;

    for (int i = 0; i < atomCount; ++i) {
        std::getline(file, line);
        Atom atom;
        std::istringstream atomss(line);
        atomss >> atom.x >> atom.y >> atom.z >> atom.type;
        atoms.push_back(atom);
    }

    for (int i = 0; i < bondCount; ++i) {
        std::getline(file, line);
        std::istringstream bondss(line);
        int idx1, idx2, type;
        bondss >> idx1 >> idx2 >> type;
        const Atom& a1 = atoms[idx1 - 1];
        const Atom& a2 = atoms[idx2 - 1];
        cilindros.emplace_back(a1.x, a1.y, a1.z, a2.x, a2.y, a2.z, 0.08f);
    }
}

void Molecule::init() {
    float radioEsfera = 0.3f;
    for (const auto& atom : atoms) {
        spheres.emplace_back(atom.x, atom.y, atom.z, radioEsfera);
        spheres.back().init();
    }
    for (auto& c : cilindros) c.init();
}

void Molecule::atomColor(const std::string& type, float& r, float& g, float& b) {
    if (type == "C")      { r = 0.2f; g = 0.2f; b = 0.2f; }
    else if (type == "O") { r = 1.0f; g = 0.0f; b = 0.0f; }
    else if (type == "N") { r = 0.0f; g = 0.0f; b = 1.0f; }
    else if (type == "H") { r = 1.0f; g = 1.0f; b = 1.0f; }
    else                  { r = 0.8f; g = 0.8f; b = 0.2f; }
}

void Molecule::draw(float rotX, float rotY) {
    for (size_t i = 0; i < spheres.size(); ++i) {
        float r, g, b;
        atomColor(atoms[i].type, r, g, b);
        spheres[i].draw(rotX, rotY, r, g, b);
    }
    for (auto& c : cilindros)
        c.draw(rotX, rotY, 0.8f, 0.8f, 0.8f);
}

void Molecule::cleanup() {
    for (auto& s : spheres) s.cleanup();
    for (auto& c : cilindros) c.cleanup();
}

