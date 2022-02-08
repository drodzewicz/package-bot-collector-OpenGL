#ifndef GAME_HPP
#define GAME_HPP

#include "constants/objectPositions.hpp"

const int rainNumber = 10000;
glm::vec3 rainPosition[rainNumber];
glm::mat4x4 rainModels[rainNumber];

int packageNumber = 20;
glm::mat4x4 packageMatModels[NUMBER_OF_PACKAGES];
glm::vec3 packageRandPositions[NUMBER_OF_PACKAGES];

void initRain () {
    for (int i = 0; i < rainNumber; ++i)
    {
        float x = (float) (rand() % 100 - 50);
        float y = (float) (rand() % 20 + 5);
        float z = (float) (rand() % 100 - 50);
        rainPosition[i] = glm::vec3(x,y,z);

        // x = Frame;
        rainModels[i] = glm::mat4x4(1.0);
        rainModels[i]  = glm::translate(rainModels[i], rainPosition[i]);
    }
}

void initGame () {
  
    // GENERATE RANDOM PACKAGES
    std::vector<int> randomIndexes = randUnique(packageNumber, NUMBER_OF_PACKAGES);
    int i = 0;
    for(int indx: randomIndexes) {
        packageRandPositions[i] = packagePositions[indx];
        i++;
    }
}

void restartGame () {
    initGame();
    robot.setPosition(robotPosition);
    robot.Reset();
    for (int i = 0; i < packageNumber; i++)
    {
        packageMatModels[i] = glm::translate(glm::mat4x4(1.0), packageRandPositions[i]);
        robot.addPackageCollider(CAABBCollider(&packageRandPositions[i], packageDimensions));
    }
}

void gameState() {
    if(robot.terminalPackageCount == packageNumber) {
        restartGame();
        textTimer = 3;
    }
    if(textTimer > 0) textTimer -= 0.01;
}

#endif
