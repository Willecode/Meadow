#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoords;

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
struct Material {
    float shininess;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D opacity_map;
    bool diffuse_map_present;
    bool specular_map_present;
    bool opacity_map_present;
};

#define MAX_POINT_LIGHTS 30
#define MAX_DIR_LIGHTS 5
uniform int pointLightCount;
uniform int dirLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight dirLight[MAX_DIR_LIGHTS];

uniform vec3 viewPos;
uniform Material material;

// Lighting flag:
// false = phong 
// true = blinn-phong
uniform bool blinn;

vec3 calcPointLight(PointLight light, vec3 normalDir, vec3 fragPos, vec3 fragToCamDir, vec3 materialDiffuse, vec3 materialSpecular);
vec3 calcDirLight(DirectionalLight light, vec3 normalDir, vec3 fragPos, vec3 fragToCamDir, vec3 materialDiffuse, vec3 materialSpecular);
vec3 calcDiffuse(vec3 fragToLightDir, vec3 lightDiffuse, vec3 materialDiffuse, vec3 normalDir);
vec3 calcSpecular(vec3 fragToLightDir, vec3 lightSpecular, vec3 materialSpecular, float shininess, vec3 normalDir, vec3 fragToCamDir);



void main()
{

    vec4 outputCol = vec4(0.0, 0.0, 0.0, 1.0);
    // Check for opacity map
    if (material.opacity_map_present)
        outputCol.a = texture(material.opacity_map, TexCoords).r;

    vec3 fragToCamDir = normalize(viewPos - fragPos);
    vec3 normalDir = normalize(normal);
    
    // Check if textures are present for diffuse and specular
    // ----
    vec3 materialDiffuse = material.diffuse;
    vec3 materialSpecular = material.specular;
    if (material.diffuse_map_present){
        materialDiffuse *= vec3(texture(material.diffuse_map, TexCoords));
    }
    if(material.specular_map_present)
        materialSpecular *= vec3(texture(material.specular_map, TexCoords)) * material.specular;
    // ----



    for (int i = 0; i < pointLightCount; i++){
        outputCol.rgb += calcPointLight(pointLights[i], normalDir, fragPos, fragToCamDir, materialDiffuse, materialSpecular);
    }
    for (int i = 0; i < dirLightCount; i++){
        outputCol.rgb += calcDirLight(dirLight[i], normalDir, fragPos, fragToCamDir, materialDiffuse, materialSpecular);
    }
    
    // DEBUG ***************************************
//    outputCol = texture(material.diffuse_map, TexCoords);
//    outputCol.a = 1;
//    if (material.opacity_map_present)
     //outputCol = vec4(normalize(normal), 1.0);

    //outputCol = material.specular;
    //outputCol = dirLight[0].direction;

//    DirectionalLight debugDirLight;
//    debugDirLight.direction = vec3(-1.0f);
//    debugDirLight.ambient = vec3(0.2);
//    debugDirLight.diffuse = vec3(0.5);
//    debugDirLight.specular = vec3(0.6);
//    outputCol += calcDirLight(debugDirLight, normalDir, fragPos, fragToCamDir, materialDiffuse, materialSpecular);
//    outputCol = vec4(texture(material.diffuse_map, TexCoords).a);
//    if (material.opacity_map_present)
//        outputCol = vec4(texture(material.opacity_map, TexCoords));
//    else
//        outputCol = vec4(1.0);
    // ***************************************

    FragColor = outputCol;
}

vec3 calcPointLight(PointLight light, vec3 normalDir, vec3 fragPos, vec3 fragToCamDir, vec3 materialDiffuse, vec3 materialSpecular){

    vec3 fragToLightDir = normalize(light.position - fragPos);
    vec3 ambient = materialDiffuse * light.ambient;
    vec3 diffuse = calcDiffuse(fragToLightDir, light.diffuse,materialDiffuse, normalDir);
    vec3 specular = calcSpecular(fragToLightDir, light.specular, materialSpecular, material.shininess, normalDir, fragToCamDir);

    // Light attenuation
    // -----------------
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		            light.quadratic * (distance * distance));
    // -----------------

    return (ambient + diffuse + specular) * attenuation;
}
vec3 calcDirLight(DirectionalLight light, vec3 normalDir, vec3 fragPos, vec3 fragToCamDir, vec3 materialDiffuse, vec3 materialSpecular){

    vec3 fragToLightDir = normalize(-light.direction);
    vec3 ambient = materialDiffuse * light.ambient;
    vec3 diffuse = calcDiffuse(fragToLightDir, light.diffuse, materialDiffuse, normalDir);
    vec3 specular = calcSpecular(fragToLightDir, light.specular, materialSpecular, material.shininess, normalDir, fragToCamDir);
    return (ambient + diffuse + specular);
}

vec3 calcDiffuse(vec3 fragToLightDir, vec3 lightDiffuse, vec3 materialDiffuse, vec3 normalDir){
    float diff = max(dot(fragToLightDir, normalDir), 0.0);
    vec3 diffuse =  lightDiffuse * (diff) * materialDiffuse;
    return diffuse;
}
vec3 calcSpecular(vec3 fragToLightDir, vec3 lightSpecular, vec3 materialSpecular, float shininess, vec3 normalDir, vec3 fragToCamDir){
    // Prevent spec on back-surfaces
    if (dot(-fragToLightDir, normalDir) > 0)
            return vec3(0.0);

    // Calculate spec component
    float spec;
    if (blinn){ // blinn-phong shading
        vec3 halfwayDir = normalize(fragToLightDir + fragToCamDir);
        spec = pow(max(dot(normalDir, halfwayDir), 0.0), shininess);
    }
    else { // phong shading
        vec3 reflectDir = reflect(-fragToLightDir, normalDir);
        spec = pow(max(dot(fragToCamDir, reflectDir), 0.0), shininess);
    }
    // Calculate specular
    vec3 specular = lightSpecular *  spec * materialSpecular;
    // debug
    //specular = vec3(spec);
    return specular;
    
}
