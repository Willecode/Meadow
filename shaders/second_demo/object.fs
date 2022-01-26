#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 objectCol;
uniform vec3 lightCol;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // Ambient
    // -------
    float ambientFac = 0.2;
    vec3 ambient = lightCol * ambientFac;

    // Diffuse
    // -------
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 diffuse = max(dot(normalDir, lightDir), 0.0) * lightCol;

    // Specular
    // --------
    float specularFac = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * objectCol * lightCol;

    vec3 finalCol = (ambient + diffuse + specular) * objectCol;
    FragColor = vec4(finalCol, 1.0);
}