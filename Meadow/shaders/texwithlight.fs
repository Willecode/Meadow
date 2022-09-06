#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoords;
in mat3 TBN;

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
    sampler2D normal_map;
    bool diffuse_map_present;
    bool specular_map_present;
    bool opacity_map_present;
    bool normal_map_present;
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
vec3 getNormalDir();

void main()
{
    // Base output color
    // ------------------
    vec4 outputCol = vec4(0.0, 0.0, 0.0, 1.0);

    // Check for opacity map
    // ------------------
    if (material.opacity_map_present)
        outputCol.a = texture(material.opacity_map, TexCoords).r;
    
    // frag -> Camera direction
    // ------------------
    vec3 fragToCamDir = normalize(viewPos - fragPos);

    // Normal vector direction
    // ------------------
    vec3 normalDir = getNormalDir();
        
    // Diffuse and specular colors
    // ------------------
    vec3 materialDiffuse = material.diffuse;
    vec3 materialSpecular = material.specular;
    if (material.diffuse_map_present){
        materialDiffuse *= vec3(texture(material.diffuse_map, TexCoords));
    }
    if(material.specular_map_present)
        materialSpecular *= vec3(texture(material.specular_map, TexCoords)) * material.specular;

    // Calculate lightsources' contribution to fragment color
    // ------------------
    for (int i = 0; i < pointLightCount; i++){
        outputCol.rgb += calcPointLight(pointLights[i], normalDir, fragPos, fragToCamDir, materialDiffuse, materialSpecular);
    }
    for (int i = 0; i < dirLightCount; i++){
        outputCol.rgb += calcDirLight(dirLight[i], normalDir, fragPos, fragToCamDir, materialDiffuse, materialSpecular);
    }
    
    // Quick DEBUG
    // ------------------
//    outputCol = texture(material.diffuse_map, TexCoords);
//    if (material.normal_map_present){
//        outputCol = texture(material.normal_map, TexCoords);
//    }
//    outputCol.a = 1;
    //outputCol.xyz = B;
    //outputCol.xyz = normalDir;
    
    // Color out
    // ------------------
    FragColor = outputCol;
}
// ------------------------------------------------------------------------
// Get normal direction from normal map or vs output
vec3 getNormalDir(){
    vec3 dir = normalize(normal);

    if (material.normal_map_present){
        dir = texture(material.normal_map, TexCoords).rgb;
        dir = (dir * 2.0) - 1.0;   
        dir = normalize(TBN * dir);
    }

    return dir;
}
// ------------------------------------------------------------------------
// Calculate pointlight color contribution
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
// ------------------------------------------------------------------------
// Calculate directional light color contribution
vec3 calcDirLight(DirectionalLight light, vec3 normalDir, vec3 fragPos, vec3 fragToCamDir, vec3 materialDiffuse, vec3 materialSpecular){

    vec3 fragToLightDir = normalize(-light.direction);
    vec3 ambient = materialDiffuse * light.ambient;
    vec3 diffuse = calcDiffuse(fragToLightDir, light.diffuse, materialDiffuse, normalDir);
    vec3 specular = calcSpecular(fragToLightDir, light.specular, materialSpecular, material.shininess, normalDir, fragToCamDir);
    return (ambient + diffuse + specular);
}
// ------------------------------------------------------------------------
// Calculate light diffuse component
vec3 calcDiffuse(vec3 fragToLightDir, vec3 lightDiffuse, vec3 materialDiffuse, vec3 normalDir){
    float diff = max(dot(fragToLightDir, normalDir), 0.0);
    vec3 diffuse =  lightDiffuse * (diff) * materialDiffuse;

    //debug
    // ---------
    //diffuse = vec3(0);
    // ---------
    return diffuse;
}
// ------------------------------------------------------------------------
// Calculate light specular component
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
    //specular = vec3(vec3(0));
    return specular;
    
}
