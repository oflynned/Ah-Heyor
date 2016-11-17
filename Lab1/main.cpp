//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "Shader.cpp"
#include "Model.cpp"
#include "Light.cpp"
#include "File.cpp"
#include "Camera.cpp"
#include "Player.cpp"
#include "Scene.cpp"
#include "Scenery.cpp"
#include "Syringe.cpp"
#include "Skybox.cpp"
#include "Sound.cpp"

#include "GLIncludes.h"

//player
#define MESH_PLAYER "player/player.obj"

//props
#define MESH_GROUND "plane/plane.obj"
#define MESH_ROCK "rock/rock.obj"
#define MESH_TREE "pine/pine.obj"
#define MESH_CAR ""
#define MESH_CANS ""

//enable debugging
#define DEV_MODE true

using namespace std;

Shader shader, skyboxShader;
Camera camera;
Light light;

Player player;
Scenery rock, tree;
Scene scene;
Skybox skybox;

std::vector<Scenery> trees;
std::vector<Scenery> rocks;
std::vector<Syringe> syringes;

Sound ramblings, music, sfx;

float last_x = 0.0f, last_y = 0.0f;

GLuint shaderProgramID;
float delta;
int width = DEV_MODE ? 800 : 1920;
int height = DEV_MODE ? 600 : 1080;

void display() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 view = camera.getView();
	mat4 persp_proj = perspective(camera.getFOV(), (float)width / (float)height, 0.1f, 200.0f);
	mat4 model = identity_mat4();

	// draw regular objects
	shader.use();

	int view_mat_location = glGetUniformLocation(shader.getProgram(), "view");
	int proj_mat_location = glGetUniformLocation(shader.getProgram(), "projection");
	int matrix_location = glGetUniformLocation(shader.getProgram(), "model");
	int light_pos_location = glGetUniformLocation(shader.getProgram(), "lightPos");
	int view_pos_location = glGetUniformLocation(shader.getProgram(), "viewPos");

	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(light_pos_location, 1, GL_FALSE, light.getLocation().m);
	glUniformMatrix4fv(view_pos_location, 1, GL_FALSE, camera.getView().m);

	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	
	player.draw(shader, matrix_location);
	rock.draw(shader, matrix_location);
	tree.draw(shader, matrix_location);
	scene.draw(shader, matrix_location);

	// skybox time
	glDepthMask(GL_FALSE);
	skyboxShader.use();

	view_mat_location = glGetUniformLocation(skyboxShader.getProgram(), "view");
	proj_mat_location = glGetUniformLocation(skyboxShader.getProgram(), "projection");
	int skybox_tex_location = glGetUniformLocation(skyboxShader.getProgram(), "skybox");

	persp_proj = perspective(camera.getFOV(), (float)width / (float)height, 0.1f, 400.0f);

	// cube it up
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, camera.getView().m);
	
	skybox.draw();
	glDepthMask(GL_TRUE);

	glutSwapBuffers();
}

void updateScene() {
	// cap at 60 fps
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f) delta = 0.03f;
	last_time = curr_time;

	scene.update();
	rock.update();
	tree.update();

	camera.update(player);
	player.update(camera.getYaw(), true);
	
	if(!DEV_MODE)
		ramblings.shouldPlay();

	// Draw the next frame
	glutPostRedisplay();
}

void init() {
	shader = Shader((GLchar*)File::getAbsoluteShaderPath("simpleShader.vert").c_str(),
		(GLchar*)File::getAbsoluteShaderPath("simpleShader.frag").c_str());

	skyboxShader = Shader((GLchar*)File::getAbsoluteShaderPath("skybox.vert").c_str(),
		(GLchar*)File::getAbsoluteShaderPath("skybox.frag").c_str());

	rock = Scenery(vec3(5.0f, -0.25f, 5.0f), MESH_ROCK);
	player = Player(vec3(2.0f, -0.5f, 2.0f), MESH_PLAYER);
	tree = Scenery(vec3(-5.0f, -0.5f, 5.0f), MESH_TREE);
	scene = Scene(vec3(0.0f, -1.5f, 0.0f), MESH_GROUND, 10.0f);

	Camera camera = Camera(player);

	if (!DEV_MODE)
		music.playAudio(Sound::music);

	skybox.init();
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {

	if (key == 27) {
		glutExit();
	}

	if (key == 'q') {
		sfx.playAudio(Sound::sfx);
	}

	player.onKey(key, camera.getTheta());
}

void releaseKeypress(unsigned char key, int x, int y) {
	player.onKeyRelease(key, camera.getTheta());
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
	glutCreateWindow("Lab 6 - Ah Heyor!");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(releaseKeypress);
	glutPassiveMotionFunc(mouseMove);
	glutMouseWheelFunc(mouseRoll);

	//hide cursor for camerav
	if (!DEV_MODE) {
		glutSetCursor(GLUT_CURSOR_NONE);
		glutFullScreen();
	}

	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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