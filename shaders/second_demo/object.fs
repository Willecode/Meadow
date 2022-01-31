#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoords;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    // -------
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // Diffuse
    // -------
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normalDir), 0.0);
    vec3 diffuse =  light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular
    // --------
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float spec = 0.0;
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular *  (spec * vec3(texture(material.specular, TexCoords)) * 1.5);

    vec3 finalCol = ambient + diffuse + specular;

    // vec3 red = vec3(1.0, 0.0, 0.0); 
    // vec3 up = vec3(0.0, 1.0, 0.0);
    // vec3 right = vec3(1.0, 0.0, 0.0);
    // if(dot(normalDir, up) == 1.0){
    //     finalCol = red;
    // }
    // else if(dot(normalDir, right) == 1.0){
    //     finalCol = up;
    // }
    FragColor = vec4(finalCol, 1.0);
}