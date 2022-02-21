#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoords;

void main()
{
    vec3 col = vec3(1.0, 1.0, 1.0);
    FragColor = vec4(col, 1.0);
}