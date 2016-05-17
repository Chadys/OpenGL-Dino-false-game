#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

uniform float fade;

void main()
{    
    color = vec4(textColor, texture(text, TexCoords).r) * fade;
}  