#version 460 core

in vec3 aPosition;
in vec3 aColor;
in vec2 aTexCoord;

out vec3 vColor;
out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uPerspect;

void main()
{
    gl_Position = uPerspect * uView * uModel * vec4(aPosition, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
}