#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoords;

uniform vec3 color;
uniform sampler2D texture_map;

void main()
{
    vec3 red = vec3(1.0, 0.0, 0.0);
    FragColor = vec4(color, 1.0);
    //FragColor = vec4(vec3(texture(texture_map, TexCoords)), 1.0);
    
}