#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoords;
in mat3 TBN;

in vec4 shadowCoord;

uniform vec3 viewPos;

// Materials
// ----------------------------------------------------------------------------

struct Material {
    // In case of no relevant maps
    // ------------------------
    vec4 albedo;
    float metallic;
    float roughness;
    // ------------------------
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    // ------------------------
    bool hasAlbedoMap;
    bool hasNormalMap;
    bool hasMetallicMap;
    bool hasRoughnessMap;
    bool hasAoMap;
    // ------------------------
    vec4 baseColorFactor;
    float metallicFactor;
    float roughnessFactor;
};
uniform Material material;

// Lights 
// ----------------------------------------------------------------------------
struct PointLight {
    vec3 position;
    vec3 color;

    // Attenuation related
    // ------------------------
    float constant;
    float linear;
    float quadratic;
    // ------------------------

};
#define MAX_POINT_LIGHTS 30
uniform int pointLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;
uniform sampler2DShadow shadowMap;
uniform float shadowBias;
uniform bool dirLightActive;

const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
vec3 getNormalDir(){
    vec3 dir = normalize(normal);

    if (material.hasNormalMap){
        dir = texture(material.normalMap, TexCoords).rgb;
        dir = (dir * 2.0) - 1.0;   
        dir = normalize(TBN * dir);
    }

    return dir;
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 getAlbedo(){
    if (material.hasAlbedoMap)
        return texture(material.albedoMap, TexCoords).rgb * material.baseColorFactor.rgb;
    else
        return material.albedo.rgb;
}
// ----------------------------------------------------------------------------
float getMetallic(){
    if (material.hasMetallicMap)
        return texture(material.metallicMap, TexCoords).r * material.metallicFactor;
    else
        return material.metallic;
}
// ----------------------------------------------------------------------------
float getRoughness(){
    if (material.hasRoughnessMap)
        return texture(material.roughnessMap, TexCoords).r * material.roughnessFactor;
    else
        return material.roughness;
}
// ----------------------------------------------------------------------------
float getAo(){
    if (material.hasAoMap)
        return texture(material.aoMap, TexCoords).r;
    else
        return 1;
}
// ----------------------------------------------------------------------------

float getOpacity(){
    if (material.hasAlbedoMap)
        return texture(material.albedoMap, TexCoords).a * material.baseColorFactor.a;
    else
        return 1;
}
// ----------------------------------------------------------------------------

float getVisibility(){
    vec3 projCoords = shadowCoord.xyz;
    projCoords = projCoords * 0.5 + 0.5;

    // just one sample
    // float visibility = texture( shadowMap, vec3(projCoords.xy, projCoords.z - shadowBias) );

    // Multiple samples
    int samples = 4;
    vec2 poissonDisk[4] = vec2[](
        vec2( -0.94201624, -0.39906216 ),
        vec2( 0.94558609, -0.76890725 ),
        vec2( -0.094184101, -0.92938870 ),
        vec2( 0.34495938, 0.29387760 )
    );
    float visibility = 0.0;
    for (int i = 0;i < samples; i++){
        float pixelVis = texture( shadowMap, vec3((projCoords.xy + poissonDisk[i]/700.0 ), projCoords.z - shadowBias));
        visibility += pixelVis;
    }

    return visibility / float(samples);
}

void main()
{	
    // Check for texture maps
    // -----------------------------
    vec3 albedo      = getAlbedo();
    float metallic   = getMetallic();
    float roughness  = getRoughness();
    float ao         = getAo();
    float opacity    = getOpacity();
    // -----------------------------

    vec3 N = getNormalDir();
    vec3 V = normalize(viewPos - fragPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    ///////////////////// Point lights
    for(int i = 0; i < pointLightCount; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(pointLights[i].position - fragPos);
        vec3 H = normalize(V + L);
        float distance = length(pointLights[i].position - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }
    /////////////////////

    ///////////////////// Directional light
    if (dirLightActive){
        vec3 L = normalize(-dirLightDir);
        vec3 H = normalize(V + L);
        //float distance = length(pointLights[i].position - fragPos);
        //float attenuation = 1.0 / (distance * distance);
        vec3 radiance = dirLightColor; // dirLightColor

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;

        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0);      
    
        float visibility = getVisibility();
        Lo += (kD * albedo / PI + specular) * radiance * NdotL * visibility;
    }
    
    /////////////////////

    vec3 ambient = vec3(0.01) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    // DEBUG
    // ------------
    //color = N;
//    color.x = roughness;
//    color.y = roughness;
//    color.z = roughness;
    // ------------
//    color = vec3(shadow);
    FragColor = vec4(color, opacity);
}