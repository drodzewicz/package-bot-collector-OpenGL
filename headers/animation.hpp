#ifndef ANIMATIONS_H
#define ANIMATIONS_H

int fps;
float Time = 0.0f;

void calculateFPS()
{
    static float framesPerSecond = 0.0f;
    static float lastTime = 0.0f;
    float currentTime = GetTickCount() * 0.001f;
    ++framesPerSecond;
    if (currentTime - lastTime > 1.0f)
    {
        lastTime = currentTime;
        fps = (int)framesPerSecond;
        framesPerSecond = 0;
    }
}

void packageSpinAnimation () {
    int packageIndex = 0;
    glm::mat4x4 tempPackageMatModels[NUMBER_OF_PACKAGES];
    for (int i = 0; i < packageNumber; i++)
    {
        bool hasPackage = false;
        for (int pkgInx : robot.collectedPackages)
        {
            hasPackage = hasPackage || i == pkgInx;
        }
        if (!hasPackage)
        {
            glm::vec3 newPos = packageRandPositions[i];
            newPos.y += abs(sin(Time * 0.01)) ; 
            tempPackageMatModels[packageIndex] = glm::translate(glm::mat4x4(1.0), newPos);
            tempPackageMatModels[packageIndex] = glm::rotate(tempPackageMatModels[packageIndex], Time * 0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
            packageIndex++;
        }
    }
    package.setupModelMesh(&tempPackageMatModels[0]);
}

void rainAnimation () {
    glm::mat4x4 tempRainModels[rainNumber];
    for (int i = 0; i < rainNumber; i++)
    {
        if(rainPosition[i].y < 2) rainPosition[i].y = 20;
        else rainPosition[i].y -= 0.15;
        
        tempRainModels[i] = glm::translate(glm::mat4x4(1.0), rainPosition[i]);
    }
    rain.setupModelMesh(&tempRainModels[0]);

}

void Animation(int keyframe)
{
    Time++;
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, Animation, 0);
    calculateFPS();
    packageSpinAnimation();
    rainAnimation();
}

#endif
