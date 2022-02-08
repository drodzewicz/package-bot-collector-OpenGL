#ifndef __MATERIAL
#define __MATERIAL

#include "../libs.hpp"

MaterialParam material_gold = {
    glm::vec3(0.24725, 0.1995, 0.0745),
    glm::vec3(0.75164, 0.60648, 0.22648),
    glm::vec3(0.628281, 0.555802, 0.366065),
    51.2f
};
MaterialParam material_green_plastic = {
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.1, 0.35, 0.1),
    glm::vec3(0.45, 0.55, 0.45),
    32.0f
};
MaterialParam material_red_rubber = {
    glm::vec3(0.05, 0.0, 0.0),
    glm::vec3(0.5, 0.4, 0.4),
    glm::vec3(0.7, 0.04, 0.04),
    10.0f
};
MaterialParam material_green_rubber = {
    glm::vec3(0.05f, 0.05f, 0.05f),
    glm::vec3(0.4f, 0.5f, 0.4f),
    glm::vec3(0.04f,0.7f,0.04f),
    10.0f
};
	

MaterialParam material_chrome = {
    glm::vec3(0.25f, 0.25f, 0.25f),
    glm::vec3(0.4f, 0.4f, 0.4f),
    glm::vec3(0.774597f, 0.774597f, 0.774597f),
    76.8f
};
#endif