#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    sampler2D diffuseMap;
    sampler2D specularMap;
};

struct DirectionalLight
{
    vec3 direction;
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight
{
    vec3 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3  position;
    vec3  direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float cutOff;
    float innercutOff;
    float outerCutOff;
};

#ifdef NR_POINT_LIGHTS
uniform PointLight          pointlight[NR_POINT_LIGHTS];
#endif

#ifdef NR_DIRECTIONAL_LIGHTS
uniform DirectionalLight    directionallight[NR_DIRECTIONAL_LIGHTS];
#endif

#ifdef NR_SPOT_LIGHTS
uniform SpotLight           spotlight[NR_SPOT_LIGHTS];
#endif

uniform Material material;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec4 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightPos = light.position;
    vec3 lightDir = normalize(lightPos - FragPos);

    // diffuse 
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
    vec4 ambient  = light.ambient * material.ambient;

#ifdef MATERIAL_DIFFUSE_MAP
    vec4 diffuse  = light.diffuse * diff * texture(material.diffuseMap, TexCoord);
#else
    vec4 diffuse  = light.diffuse * diff * material.diffuse;
#endif

#ifdef MATERIAL_SPECULAR_MAP
    vec4 specular = light.specular * spec * texture(material.specularMap, TexCoord);
#else
    vec4 specular = light.specular * spec * material.specular;
#endif
    

    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;  
    return (ambient + diffuse + specular);
}

vec4 CalcDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec4 ambient  = light.ambient * material.ambient;

#ifdef MATERIAL_DIFFUSE_MAP
    vec4 diffuse  = light.diffuse * diff * texture(material.diffuseMap, TexCoord);
#else
    vec4 diffuse  = light.diffuse * diff * material.diffuse;
#endif

#ifdef MATERIAL_SPECULAR_MAP
    vec4 specular = light.specular * spec * texture(material.specularMap, TexCoord);
#else
    vec4 specular = light.specular * spec * material.specular;
#endif

    return (ambient + diffuse + specular);
}

vec4 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir)
{
    vec4 color;
    vec3 lightPos = light.position;
    vec3 lightDir = normalize(lightPos - FragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    if(theta > light.cutOff) 
    {       
        // do lighting calculations
        // diffuse 
        float diff = max(dot(norm, lightDir), 0.0);
        
        // specular
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            
        vec4 ambient  = light.ambient * material.ambient;
 
#ifdef MATERIAL_DIFFUSE_MAP
    vec4 diffuse  = light.diffuse * diff * texture(material.diffuseMap, TexCoord);
#else
    vec4 diffuse  = light.diffuse * diff * material.diffuse;
#endif

#ifdef MATERIAL_SPECULAR_MAP
    vec4 specular = light.specular * spec * texture(material.specularMap, TexCoord);
#else
    vec4 specular = light.specular * spec * material.specular;
#endif

        diffuse  *= intensity;
        specular *= intensity;

        color = ambient + diffuse + specular;
    }
    else  // else, use ambient light so scene isn't completely dark outside the spotlight.
    {
    #ifdef MATERIAL_DIFFUSE_MAP
        color  = light.ambient * texture(material.diffuseMap, TexCoord);
    #else
        color = light.ambient * material.diffuse;
    #endif
    }

    return color;
}

void main()
{
#ifdef ENABLE_LIGHTING
    // define an output color value
    vec4 outcolor = vec4(0.0);

    vec3 norm = normalize(Normal);

    vec3 viewPos = vec4(view * vec4(0.0,0.0,0.0,1.0)).xyz;
    vec3 viewDir = normalize(viewPos - FragPos);

    #ifdef NR_POINT_LIGHTS
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
            outcolor = CalcPointLight(pointlight[i], norm, viewDir);
    #endif

    #ifdef NR_DIRECTIONAL_LIGHTS
        for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++)
            outcolor += CalcDirectionalLight(directionallight[i], norm, viewDir);
    #endif

    #ifdef NR_SPOT_LIGHTS
        for(int i = 0; i < NR_SPOT_LIGHTS; i++)
            outcolor += CalcSpotLight(spotlight[i], norm, viewDir);
    #endif

    FragColor = vec4(outcolor.xyz, 1.0);

#else

    #ifdef MATERIAL_DIFFUSE_MAP
        FragColor = texture(material.diffuseMap, TexCoord);
    #else
        FragColor = material.diffuse;
    #endif   

#endif
}