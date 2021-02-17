#version 450 core

vec3 color = vec3(1.0,0.0,0.0);
out vec4 outColor;
in vec2 TexCoord;
uniform sampler2D inTexture;

void main()
{
	outColor = texture(inTexture, TexCoord);
}