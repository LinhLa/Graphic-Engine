#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;
in float outOpacity;

uniform sampler2D texture1;

void main()
{
    FragColor =  texture(texture1, TexCoord) * outOpacity * ourColor.a + ourColor * outOpacity * (1 - ourColor.a);
}