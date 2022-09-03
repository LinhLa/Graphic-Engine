#version 330 core
out vec4 FragColor;
  
in vec4 outColor;
in vec2 TexCoord;
in float outOpacity;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord) * outColor;
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0) * outOpacity;
}