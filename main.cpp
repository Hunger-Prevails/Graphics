#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "shader_utils.h"
#include "vertices.h"

using namespace std;

glm::vec3 cam_pos;
glm::vec3 cam_front;
glm::vec3 cam_oben;

float time_value;
float last_value;

int last_mouse_x;
int last_mouse_y;
bool drags;

float pitch;
float yaw;

unsigned int program;

Model *backpack;

void init()
{
    glClearColor(0.2f, 0.2f, 0.2f, 0);
    glEnable(GL_DEPTH_TEST);
    glutSetCursor(GLUT_CURSOR_NONE);

    vector<char*> shaders;

    if (!load_shader("../shaders/main.vs", shaders)) exit(0);
    if (!load_shader("../shaders/main.fs", shaders)) exit(0);

    GLuint vShaderId = compileShaders(shaders[0], GL_VERTEX_SHADER);
    GLuint fShaderId = compileShaders(shaders[1], GL_FRAGMENT_SHADER);

    program = linkProgram(vShaderId, fShaderId);

    backpack = new Model("../res/backpack/backpack.obj", program, true);

    cam_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    cam_front = glm::vec3(0.0f, 0.0f, -1.0f);
    cam_oben = glm::vec3(0.0f, 1.0f, 0.0f);

    drags = false;

    pitch = 0.0f;
    yaw = -90.0f;
}

void display()
{
    last_value = time_value;

    auto millisec = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    time_value = (millisec % 1000000) / 1000.0;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 perspect = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    auto view_x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    auto view_y = sin(glm::radians(pitch));
    auto view_z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cam_front = glm::normalize(glm::vec3(view_x, view_y, view_z));

    glm::mat4 view = glm::lookAt(cam_pos, cam_pos + cam_front, cam_oben);

    setMat4(program, "uModel", model);
    setMat4(program, "uView", view);
    setMat4(program, "uPerspect", perspect);
    setVec3(program, "uCamPos", cam_pos);

    setVec3(program, "uLight.position", glm::vec3(1.2f, 1.0f, 2.0f));
    setVec3(program, "uLight.direction", glm::vec3(-1.2f, -1.0f, -2.0f));
    setVec3(program, "uLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    setVec3(program, "uLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    setVec3(program, "uLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    setFloat(program, "uLight.atten_a", 0.0f);
    setFloat(program, "uLight.atten_b", 0.0f);
    setFloat(program, "uLight.cone_a", glm::cos(glm::radians(10.0f)));
    setFloat(program, "uLight.cone_b", glm::cos(glm::radians(15.0f)));

    backpack->Draw(program);
    
    glutSwapBuffers();
}

void timer(int value)
{
    glutTimerFunc(16, timer, 0);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int xmouse, int ymouse)
{
    float delta_value = time_value - last_value;

    auto cam_links = glm::normalize(glm::cross(cam_oben, cam_front));

    if (key == 'w') cam_pos += delta_value * cam_front * 5.0f;
    if (key == 's') cam_pos -= delta_value * cam_front * 5.0f;
    if (key == 'a') cam_pos += delta_value * cam_links * 5.0f;
    if (key == 'd') cam_pos -= delta_value * cam_links * 5.0f;
}

void mouse(int button, int state, int xpos, int ypos)
{
    if (button == GLUT_RIGHT_BUTTON and state == GLUT_DOWN) {
        
        last_mouse_x = xpos;
        last_mouse_y = ypos;

        drags = true;

    } else if (button == GLUT_RIGHT_BUTTON and state == GLUT_UP) {

        drags = false;
    }
}

void motion(int xpos, int ypos)
{
    if (!drags) return;

    float xoffset = xpos - last_mouse_x;
    float yoffset = last_mouse_y - ypos;
    last_mouse_x = xpos;
    last_mouse_y = ypos;

    pitch += yoffset * 0.1f;
    yaw += xoffset * 0.1f;

    pitch = max(min(pitch, 60.0f), -60.0f);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Triangle Using OpenGL");
    glewInit();
    init();
    glutTimerFunc(16, timer, 0);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}