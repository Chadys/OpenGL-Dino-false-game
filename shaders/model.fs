#version 330 core

in vec2 TexCoords;

out vec4 fragcolor;

uniform sampler2D texture_diffuse1;
uniform bool back;
uniform float alpha;
uniform vec3 modelColor;

uniform float fade;

vec3 hsv2rgb(vec3 c);

void main()
{    
	if(back)
		fragcolor   = vec4(0,0,0,alpha); 
    else{
    	fragcolor   = vec4(hsv2rgb(modelColor), alpha) * texture(texture_diffuse1, TexCoords);
    }
	fragcolor *= fade;
}

//http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}