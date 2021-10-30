#version 330 core
layout (location = 0) in vec3   aPos;
layout (location = 1) in vec4   aColor;
layout (location = 2) in float  aOpacity;
layout (location = 3) in vec2   aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;
out float outOpacity;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 1.0);

    TexCoord = aTexCoord;
    ourColor = aColor;
    outOpacity = aOpacity;
}