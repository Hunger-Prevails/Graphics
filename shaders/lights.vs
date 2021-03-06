#version 460 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 uModel;

void main()
{
    gl_Position = uModel * vec4(aPosition, 1.0);
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vTexCoord = aTexCoord;
}