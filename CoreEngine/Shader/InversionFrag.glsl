#version 330 core
out vec4 FragColor;
  
in vec4 outColor;
in vec2 TexCoord;
in float outOpacity;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(vec3(1.0 - texture(texture1, TexCoord) * outColor), 1.0) * outOpacity;
}