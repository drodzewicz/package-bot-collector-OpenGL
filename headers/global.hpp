#ifndef CGLOBAL_HPP
#define CGLOBAL_HPP

#include "libs.hpp"
#include "utils.hpp"
#include "errorHandlers.hpp"
#include "obj_loader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "skybox.h"

// CONSTANTS
#include "constants/objectPositions.hpp"
#include "constants/colors.hpp"
#include "constants/materials.hpp"

// CLASS
#include "classes/CCam.hpp"
#include "classes/CProgram.hpp"
#include "classes/CMesh.hpp"
#include "classes/CFont.hpp"
#include "classes/CLight.hpp"
#include "classes/CMinimap.hpp"
#include "classes/COffRender.hpp"
#include "classes/CGround.hpp"
#include "classes/CPlayer.hpp"
#include "classes/CColider.hpp"


const int INIT_WINDOW_WIDTH = 900;
const int INIT_WINDOW_HEIGHT = 500;

int WINDOW_WIDTH = INIT_WINDOW_WIDTH;
int WINDOW_HEIGHT = INIT_WINDOW_HEIGHT;

// PROGRAMS
CProgram programMain;
CProgram programSimpleShader;
CProgram programRain;
CProgram programReflection;
CProgram minmapProgram;
CProgram programText;
CProgram program3DText;
CProgram * programActiveMain;
CProgram * programActiveReflection;

// OBJECTS
PackageRobot robot;
CGround roadGround;

std::vector<CMesh *> allObjectList;
std::vector<CMesh *> objectList;
CMesh skyscraper;
CMesh terrain;
CMesh packageTerminal;
CMesh package;
CMesh trafficLight;
CMesh rain;

// OFFRENDERING
COffRedner minimapTexture = COffRedner();

// TEXT
CFont generalText = CFont();
CFont AnitaText = CFont();
CFont Text3D = CFont();

float textTimer = 0;

// CAMERAS
CMinimap minimap;

CFreeCamera freeCam = CFreeCamera(glm::vec3(20.7, 22.75, 1.54), glm::vec3(-0.59, 3.6, 0));
CCamera minimapCamera = CCamera(glm::vec3(20.7, 22.75, 1.54), glm::vec3(-1.57, 0, 0));
PlayerCamera playerCam = PlayerCamera(&robot);

// LIGHT
LightParam sunLight = {
    glm::vec3(0.3, 0.3, 0.3),
    glm::vec3(1.2, 1.2, 1.2),
    glm::vec3(0.5, 0.5, 0.5)
};

glm::vec3 Light_Direction = glm::normalize(glm::vec3(0.81, -0.34, 0.47));
glm::vec3 Light_Position = glm::vec3(-67.84, 56.78, -26.93);

void SendLight(CProgram * program) {
    program->SendUniform3fv("sunLight.Ambient", sunLight.Ambient);
    program->SendUniform3fv("sunLight.Diffuse", sunLight.Diffuse);
    program->SendUniform3fv("sunLight.Specular", sunLight.Specular);

}

void CleanUp()
{
    programMain.DeleteProgram();
    programReflection.DeleteProgram();
    programSimpleShader.DeleteProgram();
    programRain.DeleteProgram();
    programText.DeleteProgram();
    program3DText.DeleteProgram();
    minmapProgram.DeleteProgram();

    for (CMesh *obj : objectList)
        obj->Cleanup();
}


#endif
