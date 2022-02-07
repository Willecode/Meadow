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
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

#define NR_POINTS_LIGHTS 4
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
    
    FragColor = vec4(outputCol, 1.0);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    // Ambient
    // -------
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // Diffuse
    // -------
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    //vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normalDir), 0.0);
    vec3 diffuse =  light.diffuse * (diff) * vec3(texture(material.diffuse, TexCoords));

    // Specular
    // --------
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular *  spec * vec3(texture(material.specular, TexCoords));

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
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // Diffuse
    // -------
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normalDir), 0.0);
    vec3 diffuse =  light.diffuse * (diff) * vec3(texture(material.diffuse, TexCoords));

    // Specular
    // --------
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular *  spec * vec3(texture(material.specular, TexCoords));
    // -----------------
    return (ambient + diffuse + specular);
}
    // // Ambient
    // // -------
    // vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // // Diffuse
    // // -------
    // vec3 normalDir = normalize(normal);
    // vec3 lightDir = normalize(light.position - fragPos);
    // //vec3 lightDir = normalize(-light.direction);
    // float diff = max(dot(lightDir, normalDir), 0.0);
    // vec3 diffuse =  light.diffuse * (diff) * vec3(texture(material.diffuse, TexCoords));

    // // Specular
    // // --------
    // vec3 viewDir = normalize(viewPos - fragPos);
    // vec3 reflectDir = reflect(-lightDir, normalDir);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular *  spec * vec3(texture(material.specular, TexCoords));

    // // Light attenuation
    // // -----------------
    // float distance    = length(light.position - fragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + 
    // 		            light.quadratic * (distance * distance));
    // // -----------------
    // vec3 finalCol = (ambient + diffuse + specular) * attenuation;
    
    // vec3 red = vec3(1.0, 0.0, 0.0); 
    // vec3 up = vec3(0.0, 1.0, 0.0);
    // if (dot(normalDir   , lightDir) > 0.75){
    //     finalCol = red;
    // }
    // vec3 right = vec3(1.0, 0.0, 0.0);
    // if(dot(normalDir, up) == 1.0){
    //     finalCol = red;
    // }
    // else if(dot(normalDir, right) == 1.0){
    //     finalCol = up;
    // }