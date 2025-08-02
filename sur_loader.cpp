
#include "sur_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool loadSurFile(const std::string& filename, glm::vec3 controlPoints[4][4]) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }
    std::string line;
    int uCount, vCount;
    std::getline(infile, line);
    std::istringstream(line) >> uCount >> vCount;
    std::getline(infile, line); // degree
    std::getline(infile, line); // u knot
    std::getline(infile, line); // v knot
    for (int i = 0; i < uCount; ++i) {
        for (int j = 0; j < vCount; ++j) {
            if (!std::getline(infile, line)) return false;
            std::istringstream iss(line);
            float x, y, z, w;
            iss >> x >> y >> z >> w;
            controlPoints[i][j] = glm::vec3(x, y, z);
        }
    }
    std::cout << "Loaded .sur file: " << filename << std::endl;
    return true;
}
