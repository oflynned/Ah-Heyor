#version 330                                                              
                                                                          
in vec3 position;														
in vec4 in_colour;

uniform mat4 transformation;

out vec4 color;																
                                                                        
void main() {																		
	gl_Position = transformation * vec4(position, 1.0f);
	color = in_colour;
}