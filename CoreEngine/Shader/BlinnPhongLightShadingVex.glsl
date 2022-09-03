#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

#ifdef NR_SHADOWN_MAP
uniform mat4 lightSpaceMatrix[NR_SHADOWN_MAP];
out vec4 FragPosLightSpace[NR_SHADOWN_MAP];
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal;
void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(normal) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

#ifdef NR_SHADOWN_MAP
    for(int i = 0; i < NR_SHADOWN_MAP; i++)
    {
        FragPosLightSpace[i] = lightSpaceMatrix[i] * vec4(FragPos, 1.0);
    }
#endif
}