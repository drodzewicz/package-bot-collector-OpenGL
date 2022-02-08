#ifndef __OBJ_POS
#define __OBJ_POS

#include "../libs.hpp"

// DIMENTIONS
glm::vec3 skyscraperDimensions = glm::vec3(5.67, 5.58, 16.8);
glm::vec3 packageDimensions = glm::vec3(0.603, 0.695, 0.39);
glm::vec3 robotDimentions = glm::vec3(1.23, 1.23, 1.31);
glm::vec3 packageTerminalDimentions = glm::vec3(0.864, 3.52, 2.52);
glm::vec3 packageTerminalAreaDimentions = glm::vec3(2, 3.52, 2.52);
glm::vec3 trafficLightsDimentions = glm::vec3(0.3, 0.3, 1.88);

glm::vec3 robotPosition = glm::vec3(33, 0, 0.6);

// TRAFFIC LIGHTS
const int NUMBER_OF_TRAFFIC_LIGHTS = 5;
glm::mat4x4 trafficLightsMatModels[NUMBER_OF_TRAFFIC_LIGHTS];

glm::vec3 trafficLightsPositions[NUMBER_OF_TRAFFIC_LIGHTS] = {
    glm::vec3(26, 4.7, -2.3),
    glm::vec3(26, 4.7, 24.6),
    glm::vec3(-37, 4.7, 24.6),
    glm::vec3(-37, 4.7, -20),
    glm::vec3(26, 4.7, -20),
};

// SKYSCRAPERS
const int NUMBER_OF_SKYSCRAPERS = 31;
glm::mat4x4 skyscraperMatModels[NUMBER_OF_SKYSCRAPERS];

glm::vec3 skyscraperPositions[NUMBER_OF_SKYSCRAPERS] = {
    glm::vec3(35, 8, 9.61),
    glm::vec3(35, 8, 30),
    glm::vec3(35, 8, 40),
    glm::vec3(35, 8, -30),
    glm::vec3(35, 11, -40),
    glm::vec3(25, 9, -42),
    glm::vec3(15, 8, -42),
    glm::vec3(5, 8, -42),
    glm::vec3(-5, 8, -42),
    glm::vec3(-22, 8, -42),
    glm::vec3(-32, 6, -42),
    glm::vec3(-50, 8, -42),
    glm::vec3(-50, 8, -32),
    glm::vec3(10, 8, 14),
    glm::vec3(0, 8, 14),
    glm::vec3(-10, 8, 14),
    glm::vec3(-20, 8, 14),
    glm::vec3(10, 7, 30),
    glm::vec3(20, 8, 30),
    glm::vec3(10, 8, 40),
    glm::vec3(20, 8, 40),
    glm::vec3(0, 8, 40),
    glm::vec3(0, 7, 30),
    glm::vec3(-10, 8, 40),
    glm::vec3(-10, 7, 30),
    glm::vec3(-20, 8, 40),
    glm::vec3(-20, 7, 30),
    glm::vec3(-30, 8, 40),
    glm::vec3(-30, 8, 30),
    glm::vec3(-50, 8, 40),
    glm::vec3(-50, 7, 30)
};

// PACKAGES
const int NUMBER_OF_PACKAGES = 22;
glm::vec3 packagePositions[NUMBER_OF_PACKAGES] = {
    glm::vec3(27, 5, 0),
    glm::vec3(7, 3, 0),
    glm::vec3(-13, 3, 0),
    glm::vec3(-33, 3, 0),
    glm::vec3(-53, 3, 0),
    glm::vec3(30, 5, -5),
    glm::vec3(30, 5, -25),
    glm::vec3(15, 5, -17),
    glm::vec3(-5, 5, -17),
    glm::vec3(-25, 5, -17),
    glm::vec3(-45, 5, -17),
    glm::vec3(-40, 5, -25),
    glm::vec3(-40, 5, -35),
    glm::vec3(30, 5, 10),
    glm::vec3(30, 5, 25),
    glm::vec3(30, 5, 40),
    glm::vec3(20, 5, 21),
    glm::vec3(0, 5, 21),
    glm::vec3(-20, 5, 21),
    glm::vec3(-40, 5, 21),
    glm::vec3(-40, 5, 40),
    glm::vec3(-40, 7, 0),
};


#endif