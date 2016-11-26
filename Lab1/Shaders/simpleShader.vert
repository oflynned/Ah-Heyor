#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

// lighting
out LIGHTING {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
} lighting;

// fog
out float visibility;
const float density = 0.007f;
const float gradient = 1.0f;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vec4 posRelToCam = view * gl_Position;

	lighting.Position = -vec3(lightPos);
	lighting.Normal = mat3(transpose(inverse(model))) * normal;
	lighting.TexCoords = texCoords;

	float distance = length(posRelToCam.xyz);
	visibility = exp(-pow((distance*density), gradient));
	visibility = clamp(visibility, 0.1f, 1.0f);
}