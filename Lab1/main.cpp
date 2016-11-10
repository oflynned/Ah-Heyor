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

#include "Shader.cpp"
#include "Model.cpp"
#include "Light.cpp"
#include "File.cpp"
#include "Camera.cpp"
#include "Player.cpp"
#include "Scene.cpp"

#include "GLIncludes.h"

#define MESH_NANOSUIT "nanosuit/nanosuit.obj"
#define MESH_PLANE "plane.dae"
#define MESH_BOX "box.dae"

using namespace std;

Shader shader;
Camera camera;

Light light;
Player player;
Scene scene;

float last_x = 0.0f, last_y = 0.0f;

GLuint shaderProgramID;
float delta;
int width = 1920;
int height = 1200;

GLfloat rotate_y = 0.0f;

void display() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();

	// Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shader.getProgram(), "model");
	int view_mat_location = glGetUniformLocation(shader.getProgram(), "view");
	int proj_mat_location = glGetUniformLocation(shader.getProgram(), "projection");
	int light_pos_location = glGetUniformLocation(shader.getProgram(), "lightPos");
	int view_pos_location = glGetUniformLocation(shader.getProgram(), "viewPos");

	// Root of the Hierarchy
	mat4 view = camera.getView();
	mat4 persp_proj = perspective(camera.getFOV(), (float)width / (float)height, 0.1, 100.0);
	mat4 model = identity_mat4();

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	//glUniformMatrix4fv(light_pos_location, 1, GL_FALSE, light.getLocation().m);
	//glUniformMatrix4fv(view_pos_location, 1, GL_FALSE, camera.getView().m);
	
	light.draw(shader, matrix_location);
	scene.draw(shader, matrix_location);
	player.draw(shader, matrix_location);

	glutSwapBuffers();
}

void updateScene() {
	// Placeholder code, if you want to work with framerate
	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f) delta = 0.03f;
	last_time = curr_time;

	scene.update(0.0f, true);

	camera.update(player);
	player.update(camera.getYaw(), false);

	// Draw the next frame
	glutPostRedisplay();
}

void init() {
	shader = Shader((GLchar*)File::getAbsoluteShaderPath("simpleShader.vert").c_str(),
		(GLchar*)File::getAbsoluteShaderPath("simpleShader.frag").c_str());

	light = Light(vec3(5.0f, 1.0f, 5.0f), MESH_BOX, 1.0f);
	player = Player(vec3(0.0f, -2.5f, 0.0f), MESH_NANOSUIT);
	Camera camera(player);

	scene = Scene(vec3(0.0f, -2.5f, 0.0f), MESH_PLANE);
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {

	if (key == 27) {
		glutExit();
	}

	player.onKey(key);
}

void mouseRoll(int button, int dir, int x, int y) {
	camera.adjustFOV((float) dir);
}

void mouseMove(int x, int y) {

	float xoffset = x - last_x;
	float yoffset = last_y - y;

	last_x = (float) x;
	last_y = (float) y;

	camera.mouseMoveThirdPerson(xoffset, yoffset);
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
	glutPassiveMotionFunc(mouseMove);
	glutMouseWheelFunc(mouseRoll);

	//hide cursor for camera
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();

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