#ifndef INCLUDE_SRC_OBJ_LOADER_HPP_
#define INCLUDE_SRC_OBJ_LOADER_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "hittables.hpp"
#include "triangle.hpp"
#include "vec3.hpp"

void load_obj(HittablesRecord& rec, std::string file_name) {
    std::ifstream file(file_name);

    std::string operation, dummy;

    std::vector<Vec3> vertex;
    while (!file.eof()) {
        file >> operation;
        if (operation == "#") {
            std::getline(file, dummy);
            continue;
        } else if (operation == "o") {
            std::getline(file, dummy);
            std::cerr << "load_obj: object name is " << dummy << ".\n";
            continue;
        } else if (operation == "vn" || operation == "vt") {
            // まだ使わないので無視
            std::getline(file, dummy);
            continue;
        } else if (operation == "v") {
            Vec3 point;
            for (int i = 0; i < 3; i++) {
                file >> point[i];
            }
            vertex.push_back(point);
            continue;
        } else if (operation == "f") {
            int triangle_vertex[3];
            int dummy;
            for (int i = 0; i < 3; i++) {
                file >> triangle_vertex[i];
                while (file.peek() == '/') {
                    char slash;
                    file >> slash >> dummy;
                }
            }
            rec.push_back(std::make_shared<triangle>(
                vertex[triangle_vertex[0] - 1], vertex[triangle_vertex[1] - 1],
                vertex[triangle_vertex[2] - 1]));
            continue;
        }
    }
}

#endif  // INCLUDE_SRC_OBJ_LOADER_HPP_
