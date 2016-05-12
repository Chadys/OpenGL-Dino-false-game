#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform bool back;
uniform float alpha;

void main()
{    
	if(back)
		color   = vec4(0,0,0,alpha); 
    else{
    	color   = vec4(texture(texture_diffuse1, TexCoords));
    }
}