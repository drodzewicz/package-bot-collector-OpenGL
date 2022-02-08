#define __CHECK_FOR_ERRORS                                                                                            \
    {                                                                                                                 \
        GLenum errCode;                                                                                               \
        if ((errCode = glGetError()) != GL_NO_ERROR)                                                                  \
            printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__, __LINE__); \
    }
#include "headers/global.hpp"
#include "headers/game.hpp"
#include "headers/animation.hpp"
#include "headers/menu.hpp"
#include "headers/input.hpp"
#include "headers/shadow.h"


void SendGeneralUniform(CProgram *program)
{
    if (playerViewState == FREE_CAM) freeCam.SendPVE(program);
    else playerCam.SendPVE(program);

    SendLightPVD(program);
    SendLight(program);
}

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    WINDOW_HEIGHT = height;
    WINDOW_WIDTH = width;

    playerCam.SetPerspective(width, height, 60.0f);
    freeCam.SetPerspective(width, height, 60.0f);
    minimapCamera.SetPerspective(width, height, 60.0f);
}

void DrawToScreen()
{
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    programActiveMain->UseProgram();
    programActiveMain->SendUniform1i("shadowMap", 1);
    SendGeneralUniform(programActiveMain);
    
    // DRAW OBJECTS -------------------------
    for (CMesh *obj : objectList)
    {
        obj->Draw(programActiveMain);
    }
    if (playerViewState != FIRST_PERSON)
    {
        robot.Draw(programActiveMain);
    }

    if(isSkybox == GL_TRUE) {
        if (playerViewState == FREE_CAM) DrawSkyBox(freeCam);
        else DrawSkyBox(playerCam);
    } 

    if(isRain == GL_TRUE) {
        programRain.UseProgram();
        SendGeneralUniform(&programRain);
        rain.Draw(&programRain);
    }

    programActiveReflection->UseProgram();
    programActiveReflection->SendUniform1i("shadowMap", 1);
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, idCubeTexture);
	programActiveReflection->SendUniform1i("tex_skybox", 2);
    SendGeneralUniform(programActiveReflection);
    skyscraper.Draw(programActiveReflection);


    // DRAW TEXT -------------------------
    Text3D.setModel(packageTerminal.matModel);
    glm::mat4 tempmatProj = playerViewState == FREE_CAM ? freeCam.matProj  : playerCam.matProj;
    glm::mat4 tempmatView = playerViewState == FREE_CAM ? freeCam.matView  : playerCam.matView;
    
    Text3D.translate(glm::vec3(0, 2, 0));
    char terminalPackageCountText[255];
    sprintf(terminalPackageCountText, "Packages: %d/%d", robot.terminalPackageCount, packageNumber);
    Text3D.Draw3DText(terminalPackageCountText, tempmatView, tempmatProj, 0.01f, color_white, 1);

    // FPS 
    if (showFPS == GL_TRUE)
    {
        char txt[255];
        sprintf(txt, "FPS: %d", fps);
        AnitaText.Draw2DText(txt, glm::vec2(WINDOW_WIDTH - 100, WINDOW_HEIGHT - 30), 0.5f, color_cyan);
    }

    // PACKAGE COUNT
    char packageCountText[255];
    sprintf(packageCountText, "packages: %d / %d", robot.packageCount, robot.maxPackageCount);
    AnitaText.Draw2DText(packageCountText, glm::vec2((WINDOW_WIDTH / 2) - 100 , 30), 0.5f, color_cyan);

    // ROUND
    if(textTimer > 0) {
        AnitaText.Draw2DText("YOU WIN!", glm::vec2((WINDOW_WIDTH / 2) - 60 , (WINDOW_HEIGHT / 2) - 60), 2.0f, color_green);
    }

    // POSITION
    if (showPos == GL_TRUE)
    {
        char posText[255];
        glm::vec3 camPosition = playerViewState == FREE_CAM ? freeCam.Position : playerCam.Position;
        sprintf(posText, "POS: ( %0.2lf, %0.2lf, %0.2lf )", camPosition.x, camPosition.y, camPosition.z);
        generalText.Draw2DText(posText, glm::vec2(20, WINDOW_HEIGHT - 30), 0.4f, color_white);
        glm::vec3 camDirection = playerViewState == FREE_CAM ? freeCam.Direction : playerCam.Direction;
        sprintf(posText, "DIR: ( %0.2lf, %0.2lf, %0.2lf )", camDirection.x, camDirection.y, camDirection.z);
        generalText.Draw2DText(posText, glm::vec2(20, WINDOW_HEIGHT - 50), 0.4f, color_white);
        glm::vec3 camAngle = playerViewState == FREE_CAM ? freeCam.Angles : playerCam.Angles;
        sprintf(posText, "ANGLES: ( %0.2lf, %0.2lf, %0.2lf )", camAngle.x, camAngle.y, camAngle.z);
        generalText.Draw2DText(posText, glm::vec2(20, WINDOW_HEIGHT - 70), 0.4f, color_white);
    }

    minmapProgram.UseProgram();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, DepthMap_Texture);
    minimap.Draw(&minmapProgram, minimapTexture.TextureBuffer);

    glUseProgram(0);
    
}

void RenderSceneCB()
{
    playerCam.UpdateMatView();
    freeCam.UpdateMatView();
    minimapCamera.UpdateMatView();

    minimapTexture.DrawToTexture(&programSimpleShader, &minimapCamera, allObjectList);

    FreeCamWalking();
    gameState();
    PlayerWalking();
    DrawToScreen();
    DrawShadowMap();

    __CHECK_FOR_ERRORS
    // CLEARING
    glUseProgram(0);
    glutSwapBuffers();
    glFlush();
}

void CreateObjects()
{
    // TERRAIN
    terrain = CMesh("objects/road_map.obj");
    terrain.addTexture("textures/road.png");
    objectList.push_back(&terrain);
    roadGround.Init(terrain.getVertices());

    // ROBOT
    robot = PackageRobot("objects/package_delivery_robot.obj", &roadGround, robotPosition);
    robot.addTexture("textures/amazon-robot.png");
    robot.addPlayerColider(
        CAABBCollider(&robot.position, robotDimentions));

    // TERMINAL
    packageTerminal = CMesh("objects/package_terminal.obj");
    packageTerminal.addTexture("textures/paczkomat.png");
    packageTerminal.position = glm::vec3(37, 6, 0.5);
    packageTerminal.rotation = glm::vec3(0, -1.5, 0);
    objectList.push_back(&packageTerminal);
    robot.addColider(CAABBCollider(&packageTerminal.position, packageTerminalDimentions));
    robot.setPackageTerminalCollider(CAABBCollider(&packageTerminal.position, packageTerminalAreaDimentions));

    // TRAFFIC LIGHT
    for (int i = 0; i < NUMBER_OF_TRAFFIC_LIGHTS; i++)
    {
        trafficLightsMatModels[i] = glm::translate(glm::mat4x4(1.0), trafficLightsPositions[i]);
        float scaleTraffic = 1.5f;
        trafficLightsMatModels[i] = glm::scale(trafficLightsMatModels[i], glm::vec3(scaleTraffic, scaleTraffic, scaleTraffic));
        robot.addColider(CAABBCollider(&trafficLightsPositions[i], trafficLightsDimentions));
    }
    trafficLight = CMesh("objects/traffic-light.obj", NUMBER_OF_TRAFFIC_LIGHTS, &trafficLightsMatModels[0]);
    trafficLight.addTexture("textures/traffic-light.png");
    // trafficLight.addMaterial(material_chrome);
    // trafficLight.addMaterial(material_gold);

    objectList.push_back(&trafficLight);

    // PACKAGE
    for (int i = 0; i < packageNumber; i++)
    {
        packageMatModels[i] = glm::translate(glm::mat4x4(1.0), packageRandPositions[i]);
        robot.addPackageCollider(CAABBCollider(&packageRandPositions[i], packageDimensions));
    }
    package = CMesh("objects/package.obj", packageNumber, &packageMatModels[0]);
    package.addTexture("textures/amazon-package.jpg");
    objectList.push_back(&package);

    // SKYSCRAPER
    for (int i = 0; i < NUMBER_OF_SKYSCRAPERS; i++)
    {
        skyscraperMatModels[i] = glm::translate(glm::mat4x4(1.0), skyscraperPositions[i]);
        robot.addColider(CAABBCollider(&skyscraperPositions[i], skyscraperDimensions));
    }
    skyscraper = CMesh("objects/skyscraper.obj", NUMBER_OF_SKYSCRAPERS, &skyscraperMatModels[0]);
    skyscraper.addTexture("textures/skyscraper.jpg");

    // RAIN
    rain = CMesh("objects/rain.obj", rainNumber, &rainModels[0]);
    rain.addTexture("textures/rain.png");

    // MINIMAP OBJECTS
    allObjectList = objectList;
    allObjectList.push_back(&robot);
    allObjectList.push_back(&skyscraper);
  
}

void Initialize()
{

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // PROGRAMS
    programMain = CProgram("shaders/main-vs.glsl", "shaders/main-fs.glsl");
    programReflection = CProgram("shaders/reflection-vs.glsl", "shaders/reflection-fs.glsl");
    programSimpleShader = CProgram("shaders/simple-vs.glsl", "shaders/simple-fs.glsl");
    programRain = CProgram("shaders/simple-vs.glsl", "shaders/rain-fs.glsl");
    minmapProgram  = CProgram("shaders/minimap-vs.glsl", "shaders/minimap-fs.glsl");
    programText = CProgram("shaders/text-vs.glsl", "shaders/text-fs.glsl");
    program3DText = CProgram("shaders/text-vs.glsl", "shaders/text-fs.glsl");
    programActiveMain = &programMain;
    programActiveReflection = &programReflection;

    // TEXTS
    generalText.InitText(&programText, "fonts/arial.ttf", 36);
    AnitaText.InitText(&programText, "fonts/anita.ttf", 36);
    Text3D.InitText(&program3DText, "fonts/arial.ttf", 36);

    // SKYBOX
    CreateSkyBox();

    minimap = CMinimap(MINIMAP_BOTTOM_RIGHT, 0.6f);

    InitMenu();
    initRain();
    initGame();
    ShadowMapInit();
    CreateObjects();

    minimapTexture.InitOffRendering();

    glBindVertexArray(0);
}

int main(int argc, char *argv[])
{
    // GLUT - initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    // CREATE WINDOW
    createWindow("Delivery Bot - Game", WINDOW_WIDTH, WINDOW_HEIGHT);

    // GLEW - initialization
    glewExperimental = GL_TRUE;
    SimpleErrorHandler(glewInit() != GLEW_OK, "ERROR::[GLEW_INIT_FAILED]");
    SimpleErrorHandler(!GLEW_VERSION_3_2, "ERROR::[FAILED_GLEW_VERSION_3_2]");

    // INPUTS
    glutIgnoreKeyRepeat(1);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutMouseWheelFunc(MouseWheel);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(SpecialKeys);
    glutSpecialUpFunc(SpecialKeysUp);

    // ANIMATION
    glutTimerFunc(0, Animation, 0);

    // INITIALIZE
    Initialize();
    // RENDER
    glutDisplayFunc(RenderSceneCB);
    // RESHAPE
    glutReshapeFunc(Reshape);
    // MAIN LOOP
    glutMainLoop();

    // CLEANING
    CleanUp();
    return 0;
}
