#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform sampler2D texture_normal1;
uniform sampler2D texture_normal2;

uniform sampler2D texture_height1;
uniform sampler2D texture_height2;

void main() {    
    color = vec4(	texture(texture_diffuse1, TexCoords) + 
					texture(texture_diffuse2, TexCoords) + 
					texture(texture_diffuse3, TexCoords));
}