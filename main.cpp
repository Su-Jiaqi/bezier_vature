
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "sur_loader.h"

glm::vec3 controlPoints[4][4];

float B(int i, float t) {
    switch (i) {
        case 0: return (1 - t) * (1 - t) * (1 - t);
        case 1: return 3 * t * (1 - t) * (1 - t);
        case 2: return 3 * t * t * (1 - t);
        case 3: return t * t * t;
        default: return 0;
    }
}

float dB(int i, float t) {
    switch (i) {
        case 0: return -3 * (1 - t) * (1 - t);
        case 1: return 3 * (1 - t) * (1 - 3 * t);
        case 2: return 3 * t * (2 - 3 * t);
        case 3: return 3 * t * t;
        default: return 0;
    }
}

float ddB(int i, float t) {
    switch (i) {
        case 0: return 6 * (1 - t);
        case 1: return -12 * (1 - t) + 6 * t;
        case 2: return 6 * (1 - 2 * t);
        case 3: return 6 * t;
        default: return 0;
    }
}

void evaluateBezierSurface(float u, float v,
                           glm::vec3& p, glm::vec3& pu, glm::vec3& pv,
                           glm::vec3& puu, glm::vec3& puv, glm::vec3& pvv) {
    p = pu = pv = puu = puv = pvv = glm::vec3(0.0f);
    for (int i = 0; i <= 3; ++i) {
        float Bu = B(i, u), dBu = dB(i, u), ddBu = ddB(i, u);
        for (int j = 0; j <= 3; ++j) {
            float Bv = B(j, v), dBv = dB(j, v), ddBv = ddB(j, v);
            glm::vec3 cp = controlPoints[i][j];
            p    += Bu * Bv * cp;
            pu   += dBu * Bv * cp;
            pv   += Bu * dBv * cp;
            puu  += ddBu * Bv * cp;
            puv  += dBu * dBv * cp;
            pvv  += Bu * ddBv * cp;
        }
    }
}

void computeCurvature(float u, float v) {
    glm::vec3 p, pu, pv, puu, puv, pvv;
    evaluateBezierSurface(u, v, p, pu, pv, puu, puv, pvv);
    glm::vec3 normal = glm::normalize(glm::cross(pu, pv));
    float E = glm::dot(pu, pu);
    float F = glm::dot(pu, pv);
    float G = glm::dot(pv, pv);
    float L = glm::dot(puu, normal);
    float M = glm::dot(puv, normal);
    float N = glm::dot(pvv, normal);
    float EG_FF = E * G - F * F;
    if (EG_FF == 0) {
        std::cerr << "Singular metric tensor at (" << u << ", " << v << ")\n";
        return;
    }
    float H = (E * N - 2 * F * M + G * L) / (2 * EG_FF);
    float K = (L * N - M * M) / EG_FF;
    float delta = std::sqrt(std::max(0.0f, H * H - K));
    float k1 = H + delta;
    float k2 = H - delta;
    std::cout << "u = " << u << ", v = " << v << std::endl;
    std::cout << "Principal curvatures: k1 = " << k1 << ", k2 = " << k2 << std::endl;
}

int main() {
    std::string filename = "example.sur";
    if (!loadSurFile(filename, controlPoints)) return -1;
    float u, v;
    std::cout << "Enter (u, v) in [0,1] x [0,1]: ";
    std::cin >> u >> v;
    computeCurvature(u, v);
    return 0;
}
