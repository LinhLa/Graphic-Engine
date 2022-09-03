#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


#ifdef NR_SHADOWN_MAP
in vec4 FragPosLightSpace[NR_SHADOWN_MAP];
uniform sampler2D shadowMap[NR_SHADOWN_MAP];
#endif

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
uniform mat4 viewInverse;


#ifdef NR_SHADOWN_MAP
float ShadowCalculation()
{
    float shadowFinal = 0.0;
    float bias = 0.005;
    for(int i = 0; i < NR_SHADOWN_MAP; i++)
    {
        // perform perspective divide
        vec3 projCoords = FragPosLightSpace[i].xyz / FragPosLightSpace[i].w;

        // transform to [0,1] range
        projCoords = projCoords * 0.5 + 0.5;

        // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        float closestDepth = texture(shadowMap[i], projCoords.xy).r;

        // get depth of current fragment from light's perspective
        float currentDepth = projCoords.z;

        // check whether current frag pos is in shadow
        shadowFinal = currentDepth - bias > closestDepth  ? 0.9 : 0.0;
    }
    return shadowFinal;
}
#endif

vec4 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, float ShadowFactor)
{
    vec3 lightPos = light.position;
    vec3 lightDir = normalize(lightPos - FragPos);

    // diffuse 
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
        
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
    
/*http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
* You can determine the linear and quadratic values (or what is visually close enough) for any range you wish to use:
* Linear = 4.5 / LightRange
* Quadratic = 75.0 / LightRange^2
* Attenuation = 1.0 / (1.0 + 4.5 + 75.0) = 0.0124
*/
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;  
    return (ambient + (1.0 - ShadowFactor) * (diffuse + specular));
}

vec4 CalcDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir, float ShadowFactor)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse factor
    float diff = max(dot(norm, lightDir), 0.0);

    // specular factor
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    // ambient factor
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
    // combine results
    return (ambient + (1.0 - ShadowFactor) * (diffuse + specular));
}

vec4 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir, float ShadowFactor)
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
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
            
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

        color = ambient + (1.0 - ShadowFactor) * (diffuse + specular);
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

    vec3 viewPos =  viewInverse[3].xyz;

    vec3 viewDir = normalize(viewPos - FragPos);

    #ifdef NR_SHADOWN_MAP
    // calculate shadow
    float ShadowFactor = ShadowCalculation();   
    #else
    float ShadowFactor = 0.0;
    #endif

    #ifdef NR_POINT_LIGHTS
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
            outcolor = CalcPointLight(pointlight[i], norm, viewDir, ShadowFactor);
    #endif

    #ifdef NR_DIRECTIONAL_LIGHTS
        for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++)
            outcolor += CalcDirectionalLight(directionallight[i], norm, viewDir, ShadowFactor);
    #endif

    #ifdef NR_SPOT_LIGHTS
        for(int i = 0; i < NR_SPOT_LIGHTS; i++)
            outcolor += CalcSpotLight(spotlight[i], norm, viewDir, ShadowFactor);
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