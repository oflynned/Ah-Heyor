#version 330                                                              
                                                                          
in vec3 position;	

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 in_color;

out vec4 color;																
                                                                        
void main() {																		
	gl_Position = projection * view * model  * vec4(position, 1.0f);
	color = vec4(in_color, 1.0f);
}