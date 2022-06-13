#version 400 core

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec4 position;

out VS_OUT
{
	vec4 color;
} vs_out;

void main()
{
	gl_Position = projMatrix * viewMatrix * worldMatrix * position;

	vs_out.color = position * 2.0 + vec4(0.5,0.5,0.5,0.0);
}

