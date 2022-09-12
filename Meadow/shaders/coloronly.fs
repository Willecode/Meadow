#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoords;
struct Material {
    vec3 albedo;
    sampler2D albedoMap;
    bool hasAlbedoMap;

};
uniform Material material;

void main()
{
    vec3 outCol = material.albedo;
    if (material.hasAlbedoMap){
        outCol = texture(material.albedoMap, TexCoords).rgb;
    }
    FragColor = vec4(outCol, 1.0);
    //FragColor = vec4(vec3(texture(diffuse_map, TexCoords)), 1.0);
    
}