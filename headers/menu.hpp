#ifndef __MENU_HPP
#define __MENU_HPP

#include "libs.hpp"

// MENU
enum mainMenuEnum { 
    EXIT,
    MINIMAP,
    FPS, 
    POS,
    RESTART,
    FIRST_PERSON,
    THIRD_PERSON,
    FREE_CAM,
    FULLSCREEN,
    SHADOW,
    SKYBOX,
    RAIN
};
enum playerViewMenu {  };

GLboolean isFullScreen = GL_FALSE;
int playerViewState = FIRST_PERSON;
GLboolean showFPS = GL_FALSE;
GLboolean showPos = GL_FALSE;
GLboolean isShadow = GL_TRUE;
GLboolean isSkybox = GL_TRUE;
GLboolean isRain = GL_FALSE;

void MenuHandler (int val) {
    switch (val)
    {
    case EXIT:
        exit(0);
        break;
    case MINIMAP:
        minimap.toggleMinimapVisibility();
        break;
    
    case FPS:
        showFPS = showFPS == GL_TRUE ? GL_FALSE : GL_TRUE;
        break;
    case POS:
        showPos = showPos == GL_TRUE ? GL_FALSE : GL_TRUE;
        break;
    case FIRST_PERSON:
        playerCam.useFirstPersonView();
        playerViewState = FIRST_PERSON;
        break;
    case THIRD_PERSON:
        playerCam.useThirdPersonView();
        playerViewState = THIRD_PERSON;
        break;
    case FREE_CAM:
        playerViewState = FREE_CAM;
        break;
    case RESTART:
        restartGame();
        break;
    case RAIN:
        isRain = isRain == GL_TRUE ? GL_FALSE : GL_TRUE;
        break;
    case SHADOW:
        isShadow = isShadow == GL_TRUE ? GL_FALSE : GL_TRUE;
        if(isShadow == GL_TRUE) {
            programActiveMain = &programMain;
            programActiveReflection = &programReflection;

        } else {
            programActiveMain = &programSimpleShader;
            programActiveReflection = &programSimpleShader;

        }

        break;
    case SKYBOX:
        isSkybox = isSkybox == GL_TRUE ? GL_FALSE : GL_TRUE;
        break;
    case FULLSCREEN:
        isFullScreen = isFullScreen == GL_TRUE ? GL_FALSE : GL_TRUE;
        if(isFullScreen == GL_TRUE) glutFullScreen();
        else {
            glutReshapeWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
            glutPositionWindow(
                (glutGet(GLUT_SCREEN_WIDTH) - INIT_WINDOW_WIDTH) / 2,
                (glutGet(GLUT_SCREEN_HEIGHT) - INIT_WINDOW_HEIGHT) / 2
                );
        }
        break;
    default:
        break;
    }
}


void InitMenu () {
    // PLAYER VIEW MENU
    int playerMenu = glutCreateMenu(MenuHandler);
    
    glutAddMenuEntry(" FIRST PERSON ", FIRST_PERSON);
    glutAddMenuEntry(" THIRD PERSON ", THIRD_PERSON);
    glutAddMenuEntry(" FREE CAM ", FREE_CAM);

    // RESET yes - no
    int restartMenu = glutCreateMenu(MenuHandler);
    glutAddMenuEntry("     YES     ", RESTART);
    glutAddMenuEntry("     NO      ", -1);

    // HUD
    int hudMenu = glutCreateMenu(MenuHandler);
    glutAddMenuEntry("     FPS [ON/FF]      ", FPS);
    glutAddMenuEntry("   POSITION [ON/FF]     ", POS);
    glutAddMenuEntry("    MINIMAP [ON/FF]    ", MINIMAP);

    // VIDEO SETTINGS
    int videoSettingsMenu = glutCreateMenu(MenuHandler);
    glutAddMenuEntry("   FULLSCREEN [ON/FF]    ", FULLSCREEN);
    glutAddMenuEntry("   SHADOW LIGHT [ON/FF]    ", SHADOW);
    glutAddMenuEntry("      SKYBOX [ON/FF]    ", SKYBOX);
    glutAddMenuEntry("      RAIN [ON/FF]    ", RAIN);

    // MAIN MENU
    glutCreateMenu(MenuHandler);
    glutAddSubMenu("        RESTART     ", restartMenu);
    glutAddSubMenu("      HUD OPTIONS      ", hudMenu);
    glutAddSubMenu("      VIDEO SETTINGS      ", videoSettingsMenu);
    glutAddSubMenu("       PLAYER VIEW       ", playerMenu);
    glutAddMenuEntry("----------------------", -1);
    glutAddMenuEntry("         EXIT         ", EXIT);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

#endif