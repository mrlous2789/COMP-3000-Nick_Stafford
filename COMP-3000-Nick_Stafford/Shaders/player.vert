#version 400 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 mvp;

out vec2 TexCoord;

void main()
{	
	//fragColor = color;
	gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0);
	gl_Position.w = 1.0;
	TexCoord = inTexCoord;
}