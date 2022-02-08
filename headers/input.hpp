#ifndef __INPUT
#define __INPUT

#include "libs.hpp"

// Camera view settings and variables
GLfloat _minimap_scene_translate_y = 0.0f;
int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;

float mouseXDiff = 0;
float mouseYDiff = 0;

int toggleMinimap = 0;

GLfloat sensitivity = 0.1f;

bool w = false, a = false, s = false, d = false;
bool arrow_l = false, arrow_r = false, arrow_u = false, arrow_d = false, space = false, shift = false, mouse_l = false;

void updateMinimapCamera(CCamera * camera, glm::vec3 offset = glm::vec3(0,0,0)) {
    minimapCamera.Position = camera->Position;

    if(playerViewState == THIRD_PERSON) {
        minimapCamera.Position.x -= -4 * sin(robot.rotation.y);
        minimapCamera.Position.z -= -4 * cos(robot.rotation.y);
    }
    minimapCamera.Angles.y = camera->Angles.y;
    minimapCamera.Position += offset;
    minimapCamera.UpdateDirectionCross();
}


// PLAYER WALKING
void PlayerWalking() {

    float speed = 0.05;

    if(playerViewState == FREE_CAM) {
        if(arrow_l) robot.rotate(speed);
        if(arrow_r) robot.rotate(-speed);
        if(arrow_u) robot.Drive(speed);
        if(arrow_d) robot.Drive(-speed);
    } else {
        speed = shift ? 2 * speed : speed;
        if(a) robot.rotate(speed);
        if(d) robot.rotate(-speed);
        if(w) robot.Drive(speed);
        if(s) robot.Drive(-speed);

        playerCam.Update();
        updateMinimapCamera(&playerCam, glm::vec3(0, 10, 0));
    }
}

void FreeCamWalking() {
    if(playerViewState == FREE_CAM) {
        if(a) freeCam.Rotate(0.0, 0.05);
        if(d) freeCam.Rotate(0.0, -0.05);
        if(w) freeCam.Move(0.2);
        if(s) freeCam.Move(-0.2);
        if(space) freeCam.Position.y += 0.2;
        if(shift) freeCam.Position.y -= 0.2;
        if(mouse_l) {
            float sens = 5.0f;
            float pitch = mouseYDiff / (float)WINDOW_HEIGHT; 
            float yaw = mouseXDiff / (float)WINDOW_WIDTH;
            freeCam.Rotate(pitch * sens, yaw * sens);
        }

        updateMinimapCamera(&freeCam, glm::vec3(0, 4, 0));
    } 

}

// KEYBOARD
void KeyboardUp(unsigned char key, int x, int y)
{ 
    if (key == 'a' || key == 'A') a = false;
    else if (key == 'd' || key == 'D') d = false;
    else if (key == 'w' || key == 'W') w = false;
    else if (key == 's' || key == 'S') s = false;
    else if (key == ' ') space = false;

}
void Keyboard(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A') a = true;
    else if (key == 'd' || key == 'D') d = true;
    else if (key == 'w' || key == 'W') w = true;
    else if (key == 's' || key == 'S') s = true;
    else if (key == ' ') space = true;
}

// SPECIAL KEYS
void SpecialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT) arrow_l = true;
    else if (key == GLUT_KEY_RIGHT) arrow_r = true;
    else if (key == GLUT_KEY_UP) arrow_u = true;
    else if (key == GLUT_KEY_DOWN) arrow_d = true;
    else if (key == GLUT_KEY_SHIFT_L) shift = true;
}
void SpecialKeysUp(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT) arrow_l = false;
    else if (key == GLUT_KEY_RIGHT) arrow_r = false;
    else if (key == GLUT_KEY_UP) arrow_u = false;
    else if (key == GLUT_KEY_DOWN) arrow_d = false;
    else if (key == GLUT_KEY_SHIFT_L) shift = false;
}

// MOUSE BUTTON
void MouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        _mouse_buttonState = state;
        mouse_l = !mouse_l;
        if (state == GLUT_DOWN)
        {
            _mouse_buttonX = x;
            _mouse_buttonY = y;

        }
    }
}

// MOUSE MOTION
void MouseMotion(int x, int y)
{
    if (_mouse_buttonState == GLUT_DOWN)
    {

        if(abs(_mouse_buttonX - x) < 10) mouseXDiff = _mouse_buttonX - x;
        if(abs(_mouse_buttonY - y) < 10) mouseYDiff = _mouse_buttonY - y;


        if(x >= WINDOW_WIDTH) glutWarpPointer(1, y);
        if(x <= 0 ) glutWarpPointer(WINDOW_WIDTH - 1, y);
        if(y >= WINDOW_HEIGHT) glutWarpPointer(x, 1);
        if(y <= 0 ) glutWarpPointer(x, WINDOW_HEIGHT - 1);
       
        _mouse_buttonX = x;
        _mouse_buttonY = y;

    }
}

// MOUSE WHEEL
void MouseWheel(int button, int dir, int x, int y)
{

}
#endif