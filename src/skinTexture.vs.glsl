#version 430 core

	layout(std430, binding = 0) buffer inversePoseMatrix
	{
		mat4 invPose[];
	};

	layout(std430, binding = 1) buffer boneWorldMatrix
	{
		mat4 boneWorld[];
	};

	uniform mat4 viewMatrix;
	uniform mat4 projMatrix;
	uniform vec3 vLightPos;
	uniform vec4 vColor;
	uniform mat4 localMatrix;

	layout (location = 0) in vec4 vert;  // x y z  (w=1)
	layout (location = 1) in vec2 uv;
	layout (location = 2) in vec3 norms;
	layout (location = 3) in vec4  boneWeight;
	layout (location = 4) in ivec4 boneIndex;

	out VS_OUT
	{
		vec4 vFragColor;
		vec2 vTex;
	} vs_out;

void main()
{
	vs_out.vTex = uv;

	mat4 skinMatrix = boneWorld[boneIndex.x] * invPose[boneIndex.x] * boneWeight.x + boneWorld[boneIndex.y] * invPose[boneIndex.y] * boneWeight.y + boneWorld[boneIndex.z] * invPose[boneIndex.z] * boneWeight.z + boneWorld[boneIndex.w] * invPose[boneIndex.w] * boneWeight.w;

	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * vColor.rgb;

	mat4 mvMatrix = viewMatrix * skinMatrix;
	mat3 mNormalMatrix;
	mNormalMatrix[0] = normalize(mvMatrix[0].xyz);
	mNormalMatrix[1] = normalize(mvMatrix[1].xyz);
	mNormalMatrix[2] = normalize(mvMatrix[2].xyz);
	vec3 vNorm = normalize(mNormalMatrix * norms);

	vec4 ecPosition = mvMatrix * vert;
	vec3 ecPosition3 = ecPosition.xyz /ecPosition.w;
	vec3 vLightDir = normalize(vLightPos - ecPosition3);

	float fDot = max(0.0, dot(vNorm, vLightDir)); 
	vs_out.vFragColor.rgb = ambient + (vColor.rgb * fDot);
	vs_out.vFragColor.a = vColor.a;

	gl_Position = projMatrix * viewMatrix * localMatrix * skinMatrix * vert;
}