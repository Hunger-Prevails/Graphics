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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader_utils.h"

using namespace std;

GLuint loadArrayBuffer(vector<float> attributes)
{
    GLuint array_buffer;

    glGenBuffers(1, &array_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
    glBufferData(GL_ARRAY_BUFFER, attributes.size() * sizeof(float), attributes.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return array_buffer;
}

GLuint loadElementBuffer()
{
    GLuint indices[] = {0, 1, 2, 1, 3, 2};

    GLuint element_buffer;

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return element_buffer;
}

GLuint load_texture(string image_file)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    auto data = stbi_load(image_file.c_str(), &width, &height, 0, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

GLuint vertex_array;
GLuint programId;
GLuint texture;

void init()
{
    texture = load_texture("../res/container.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glClearColor(0.2f, 0.2f, 0.2f, 0);
    glEnable(GL_DEPTH_TEST);

    float vertex_data[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    };
    vector<float> data;
    data.assign(vertex_data, vertex_data + 36 * 8);
    GLuint array_buffer = loadArrayBuffer(data);

    vector<char*> shaders;

    if (!load_shader("../shaders/vs.glsl", shaders)) exit(0);
    if (!load_shader("../shaders/fs.glsl", shaders)) exit(0);

    GLuint vShaderId = compileShaders(shaders[0], GL_VERTEX_SHADER);
    GLuint fShaderId = compileShaders(shaders[1], GL_FRAGMENT_SHADER);

    programId = linkProgram(vShaderId, fShaderId);

    GLuint position_attrib = glGetAttribLocation(programId, "aPosition");
    GLuint tex_coord_attrib = glGetAttribLocation(programId, "aTexCoord");
    GLuint color_attrib = glGetAttribLocation(programId, "aColor");

    glGenVertexArrays(1, &vertex_array);

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, false, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(tex_coord_attrib, 2, GL_FLOAT, false, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(color_attrib, 3, GL_FLOAT, false, 8 * sizeof(float), (void *)(5 * sizeof(float)));

    glEnableVertexAttribArray(position_attrib);
    glEnableVertexAttribArray(tex_coord_attrib);
    glEnableVertexAttribArray(color_attrib);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(programId);
    
    glBindVertexArray(vertex_array);

    int volume_uniform = glGetUniformLocation(programId, "uVolume");
    auto time_value = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    glUniform1f(volume_uniform, sin((time_value % 100000) * 3.1416 / 2000.0) / 2.0f + 0.5f);

    int sampler_uniform = glGetUniformLocation(programId, "uTexture");
    glUniform1i(sampler_uniform, 0);

    glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)((time_value % 100000) * 3.1416 / 2000.0), glm::vec3(1.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 perspect = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    int model_uniform = glGetUniformLocation(programId, "uModel");
    int view_uniform = glGetUniformLocation(programId, "uView");
    int perspect_uniform = glGetUniformLocation(programId, "uPerspect");

    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(perspect_uniform, 1, GL_FALSE, glm::value_ptr(perspect));

    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glutSwapBuffers();
}

void timer( int value )
{
    glutTimerFunc(16, timer, 0);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1600, 900);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Triangle Using OpenGL");
    glewInit();
    init();
    glutTimerFunc(16, timer, 0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}