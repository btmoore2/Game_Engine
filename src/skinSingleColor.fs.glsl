#version 400 core

in VS_OUT
{
	vec4 vFragColor;
} fs_in;

out vec4 color;

void main()
{
	color = fs_in.vFragColor;
}