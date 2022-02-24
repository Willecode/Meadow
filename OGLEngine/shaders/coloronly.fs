#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoords;

uniform vec3 objectColor;

void main()
{
    FragColor = vec4(objectColor, 1.0);
}