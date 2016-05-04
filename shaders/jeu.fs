#version 330 core
in vec2 TexCoord;

out vec4 fragcolor;

uniform sampler2D sprite;
uniform vec3 spriteColor;
uniform float spriteAlpha;

void main()
{
    vec4 color = vec4(spriteColor, spriteAlpha)*texture(sprite, TexCoord);
	if(color.a<0.1)
		discard;
    fragcolor = color;
}

