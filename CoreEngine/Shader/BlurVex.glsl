#version 330 core
layout (location = 0) in vec3   aPos;
layout (location = 1) in vec4   aColor;
layout (location = 2) in float  aOpacity;
layout (location = 3) in vec2   aTexCoord;

out vec4 outColor;
out vec2 TexCoord;
out float outOpacity;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;
    outColor = aColor;
    outOpacity = aOpacity;
}