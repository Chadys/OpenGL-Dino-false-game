#version 330 core
in vec2 TexCoord;

out vec4 fragcolor;

uniform sampler2D sprite;
uniform bool is3D;
uniform vec3 spriteColor;
uniform float spriteAlpha;
uniform uvec2 spritePos;
uniform vec2 spriteStep;

void main()
{
	vec4 color;
	if(is3D)
	    color = vec4(spriteColor, spriteAlpha)*texture(sprite, TexCoord);
	else
		color = texture(sprite, vec2(TexCoord.x+spritePos.x*spriteStep.x,TexCoord.y+spritePos.y*spriteStep.y));
	if(color.a<0.1)
		discard;
    fragcolor = color;
}

