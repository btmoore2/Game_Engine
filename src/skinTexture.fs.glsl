#version 400 core

uniform sampler2D texObject;

in VS_OUT
{
	vec4 vFragColor;
	vec2 vTex;
} fs_in;

out vec4 color;

void main()
{
	color = fs_in.vFragColor * texture(texObject, fs_in.vTex);
}