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
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};

#define NR_POINTS_LIGHTS 1
uniform PointLight pointLights[NR_POINTS_LIGHTS];
uniform DirectionalLight dirLight;
uniform vec3 viewPos;
uniform Material material;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 outputCol = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fragPos);
    
    for (int i = 0; i < pointLights.length(); i++){
        outputCol += calcPointLight(pointLights[i], normal, fragPos, viewDir);
    }
    outputCol += calcDirLight(dirLight, normal, fragPos, viewDir);
    
    // DEBUG ***************************************
    //outputCol = vec3(texture(material.specular_map, TexCoords));
    //outputCol = vec3(0.0, 1.0, 0.0);
    //outputCol = pointLights[0].ambient;
    // ***************************************
    FragColor = vec4(outputCol, 1.0);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    // Ambient
    // -------
    vec3 ambient = vec3(texture(material.diffuse_map, TexCoords)) * light.ambient;

    // Diffuse
    // -------
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    //vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normalDir), 0.0);
    vec3 diffuse =  light.diffuse * (diff) * vec3(texture(material.diffuse_map, TexCoords));

    // Specular
    // --------
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular *  spec * vec3(texture(material.specular_map, TexCoords));

    // Light attenuation
    // -----------------
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		            light.quadratic * (distance * distance));
    // -----------------

    return (ambient + diffuse + specular) * attenuation;
}
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    // Ambient
    // -------
    vec3 ambient = vec3(texture(material.diffuse_map, TexCoords)) * light.ambient;

    // Diffuse
    // -------
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normalDir), 0.0);
    vec3 diffuse =  light.diffuse * (diff) * vec3(texture(material.diffuse_map, TexCoords));

    // Specular
    // --------
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular *  spec * vec3(texture(material.specular_map, TexCoords));
    // -----------------
    return (ambient + diffuse + specular);
}
