#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular
    vec3 viewPos = (view * vec4(0.0)).xyz;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
    vec3 ambient  = (light.color * light.ambient) * texture(material.diffuseMap, TexCoord).xyz;
    vec3 diffuse  = (light.color * light.diffuse) * diff * texture(material.diffuseMap, TexCoord).xyz;
    vec3 specular = (light.color * light.specular) * spec * texture(material.specularMap, TexCoord).xyz; 

    FragColor = vec4(ambient + diffuse + specular, 1.0); 
}