#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct Light
{
    vec3 position;
 	vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform Material material;
uniform Light light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 lightPos = (model * vec4(light.position, 1.0)).xyz;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular
    vec3 viewPos = (model * view * vec4(0.0)).xyz;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
    vec4 ambient  = (light.color * light.ambient) * material.ambient;
    vec4 diffuse  = (light.color * light.diffuse) * diff * material.diffuse;
    vec4 specular = (light.color * light.specular) * spec * material.specular; 

    FragColor = ambient + diffuse + specular;
}