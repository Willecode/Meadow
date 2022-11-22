#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


uniform mat4 model;
//uniform mat4 view_shadowmap;
//uniform mat4 projection_shadowmap;
uniform mat4 shadowmapVP; // Shadowmap Proj * View matrix

void main(){

//    gl_Position = projection_shadowmap * view_shadowmap * model * vec4(aPos, 1.0);
    gl_Position = shadowmapVP * model * vec4(aPos, 1.0);
}