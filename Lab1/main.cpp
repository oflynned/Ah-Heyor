//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

// Assimp includes

#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

#include "ShaderHelper.cpp"
#include "Model.cpp"
#include "Light.cpp"
#include "File.cpp"

#include "GLIncludes.h"
#include "Shader.cpp"

#define MESH_NANOSUIT "nanosuit/nanosuit.obj"
#define MESH_MUSHROOM "mushroom.dae"
#define MESH_BOX "box.dae"
#define MESH_PLANE "plane.dae"

using namespace std;

Shader shader;
Model objModel;

GLuint shaderProgramID;
int width = 800;
int height = 600;

GLfloat rotate_y = 0.0f;

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	#pragma region GeneralShader
	shader.use();

	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shader.getProgram(), "model");
	int view_mat_location = glGetUniformLocation(shader.getProgram(), "view");
	int proj_mat_location = glGetUniformLocation(shader.getProgram(), "projection");

	// Root of the Hierarchy
	mat4 view = identity_mat4();
	view = translate(view, vec3(0.0, 0.0, -10.0f));

	mat4 persp_proj = perspective(45.0, (float)width / (float)height, 0.1, 100.0);

	mat4 model = identity_mat4();
	model = translate(model, vec3(0.0f, 0.0f, 0.0f));
	model = scale(model, vec3(0.2f, 0.2f, 0.2f));
	model = rotate_y_deg(model, rotate_y);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);

	objModel.draw(shader);
	#pragma endregion

	glutSwapBuffers();
}

void setIllumination() {

}

void updateScene() {

	// Placeholder code, if you want to work with framerate
	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f)
		delta = 0.03f;
	last_time = curr_time;

	// rotate the model slowly around the y axis
	rotate_y += 0.05f;
	// Draw the next frame
	glutPostRedisplay();
}

void init()
{
	//set up shaders
	shader = Shader(
		(GLchar*)File::getAbsoluteShaderPath("simpleVertexShader.txt").c_str(), 
		(GLchar*)File::getAbsoluteShaderPath("simpleFragmentShader.txt").c_str());

	//set up objects
	objModel = Model((GLchar*)File::getAbsoluteModelPath(MESH_NANOSUIT).c_str());
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {

	if (key == 27) {
		glutExit();
	}

}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Lab 5 - Interactive Walkthrough");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}