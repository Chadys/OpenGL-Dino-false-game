#version 330 core
in vec2 TexCoord;
in vec4 InstanceColor;

out vec4 fragcolor;

uniform sampler2D sprite;

uniform bool is3D;
uniform vec3 spriteColor;
uniform float spriteAlpha;

uniform uvec2 spritePos;
uniform vec2 spriteStep;

uniform bool border;

uniform int effect;
uniform bool isCircle;
uniform vec3 color1;
uniform vec3 color2;

uniform float fade;


vec4 create_border(vec4 color);
vec3 create_gradient();
bool inCircle();


void main()
{
	vec4 color;
	// 3D object, simply apply texture
	if(is3D)
	    color = InstanceColor*texture(sprite, TexCoord);
	else{
		// 2D sprite, obtain correct location in sprite sheet
		if(effect<0){
			color = texture(sprite, vec2(TexCoord.x+spritePos.x*spriteStep.x,TexCoord.y+spritePos.y*spriteStep.y));
			color.w = color.w*spriteAlpha;
		}
		// 2D colored square, apply correct color depending on the choosen effect
		else{
			color = vec4(create_gradient(), 1);
			if(isCircle && !inCircle())
				discard;
		}
		if(border)
			color = create_border(color);
	}
	color *= fade;
	if(color.a<0.1)
		discard;
    fragcolor = color;
}

bool inCircle(){
	if (distance(TexCoord, vec2(0.5,0.5)) > 0.5)
		return false;
	return true;
}

vec3 create_gradient(){
	switch(effect){

	// NONE
	case 0 :
		return color1;

	// RADIAL, color1 is centered
	case 1 :
		return mix(color1, color2, min(distance(TexCoord, vec2(0.5,0.5)), 1));

	// VERTICAL, color1 is up
	case 2 :
		return mix(color1, color2, distance(TexCoord, vec2(TexCoord.x,0.0)));

	// HORIZONTAL, color1 is on the left
	case 3 :
		return mix(color1, color2, distance(TexCoord, vec2(0.0,TexCoord.y)));	
	}
}

vec4 create_border(vec4 color){
	if (TexCoord.x < 0.01 || TexCoord.x > 0.99 || TexCoord.y < 0.01 || TexCoord.y > 0.99)
		return vec4(vec3(0), 1);
	if (TexCoord.x < 0.04 || TexCoord.x > 0.96 || TexCoord.y < 0.04 || TexCoord.y > 0.96)
		return mix(color, vec4(1), 0.4);
	if (TexCoord.x < 0.05 || TexCoord.x > 0.95 || TexCoord.y < 0.05 || TexCoord.y > 0.95)
		return vec4(vec3(0), 1);
	return color;
}