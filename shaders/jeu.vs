#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in mat4 instanceMatrix;
layout (location = 7) in vec3 instanceColor;

out vec2 TexCoord;
out vec4 InstanceColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool is3D;

void main()
{
	if (is3D)
		gl_Position = projection * view * instanceMatrix * vec4(position, 1.0f);
    else
    	gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord =  texCoord;
    InstanceColor = vec4(instanceColor, 1);
}

