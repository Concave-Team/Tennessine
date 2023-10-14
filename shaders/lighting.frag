#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct sMaterial
{
    float specularStrength;
    vec3 albedoColor;
    sampler2D albedoTexture;
    float Roughness;
    bool Textured;
};

struct sLight
{
    vec3 Position;
    vec3 Color;
    vec3 Direction;
    float Attenuation;
    float Intensity;
    float AmbientCoef;
};

uniform sMaterial Material;
uniform sLight Light;

uniform vec3 AmbientColor = vec3(1,1,1);
uniform float AmbientIntensity = 0.95f;
uniform vec3 viewPos;


float specularStrength = 0.5;

vec3 CalculateSpecular(sLight light)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.Position-FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.Color;

    return specular;
}

vec3 CalculateDiffuse(sLight light)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.Position-FragPos);
    return ((max(dot(norm, lightDir), 0.0)) * light.Color)*Material.Roughness;
}

void main()
{
    vec3 TotalAmbient = AmbientColor * AmbientIntensity;

    if(Material.Textured == true)
    {
        FragColor = texture(Material.albedoTexture, TexCoord) * vec4(Material.albedoColor, 1.0f) * vec4(TotalAmbient+CalculateDiffuse(Light)+CalculateSpecular(Light), 1.0f);;
    }
    else
    {
        FragColor = vec4(Material.albedoColor, 1.0f)*vec4(TotalAmbient+CalculateDiffuse(Light)+CalculateSpecular(Light), 1.0f);
    }
}                                                  