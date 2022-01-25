#version 330 core
out vec4 FragColor;

uniform vec3 objectCol;
uniform vec3 lightCol;

void main()
{
    FragColor = vec4(objectCol, 1.0);
}