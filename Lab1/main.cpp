//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "Cans.cpp"
#include "Camera.cpp"
#include "File.cpp"
#include "Light.cpp"
#include "Manager.cpp"
#include "Model.cpp"
#include "Number.cpp"
#include "Player.cpp"
#include "Scene.cpp"
#include "Scenery.cpp"
#include "Shader.cpp"
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
#define MESH_BOX "box/box.obj"
#define MESH_CANS "can/can.obj"

//numbers for UI
#define MESH_ZERO "text/0/0.obj"
#define MESH_ONE "text/1/1.obj"
#define MESH_TWO "text/2/2.obj"
#define MESH_THREE "text/3/3.obj"
#define MESH_SLASH "text/slash/slash.obj"

//macro offset
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

Shader shader, skyboxShader, guiShader;
Camera camera;
Light light;

Player player;
Scenery rock, tree;
Scene scene;
Skybox skybox;

Number gui_zero, gui_one, gui_two, gui_three, gui_slash;

std::vector<Scenery> trees;
std::vector<Scenery> rocks;
std::vector<Cans> cans;

bool toggleFog = false;

Sound ramblings, music, sfx;

float last_x = 0.0f, last_y = 0.0f;

float delta;
int width = DEV_MODE ? 800 : 1920;
int height = DEV_MODE ? 600 : 1080;

float getHealthPercentage(Player& player) {
	return (float)player.getHealth() / (float)TOTAL_HEALTH;
}

void drawText(std::string text, int x, int y, int z) {
	const char* textToDraw = (const char*)text.c_str();
	int length = text.size();

	glColor3f(0, 1, 0);
	glMatrixMode(GL_PROJECTION);
	double * matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos3i(x, y, z);

	for (int i = 0; i < length; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)textToDraw[i]);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void drawGUI() {
	glRotatef(-player.getYRot(), player.getPos().v[0], player.getPos().v[1], player.getPos().v[2]);

	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);

	glVertex3f(player.getPos().v[0] + 1.0, player.getPos().v[1] + 4.0, player.getPos().v[2]);
	glVertex3f(player.getPos().v[0] - 1.0 * getHealthPercentage(player), player.getPos().v[1] + 4.0, player.getPos().v[2]);
	glVertex3f(player.getPos().v[0] - 1.0 * getHealthPercentage(player), player.getPos().v[1] + 4.25, player.getPos().v[2]);
	glVertex3f(player.getPos().v[0] + 1.0, player.getPos().v[1] + 4.25, player.getPos().v[2]);

	glEnd();
	glFlush();

	// draw 3
	mat4 view = camera.getView();
	mat4 persp_proj = perspective(camera.getFOV(), (float)width / (float)height, 0.1f, 1000.0f);

	guiShader.use();

	int view_mat_location = glGetUniformLocation(guiShader.getProgram(), "view");
	int proj_mat_location = glGetUniformLocation(guiShader.getProgram(), "projection");
	int model_matrix_location = glGetUniformLocation(guiShader.getProgram(), "model");
	int colour_location = glGetUniformLocation(guiShader.getProgram(), "in_color");

	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	
	glUniform3fv(colour_location, 1, vec3(1.0, 0.0, 0.0).v);
	gui_zero.draw(guiShader, model_matrix_location);

	glUniform3fv(colour_location, 1, vec3(0.0, 1.0, 0.0).v);
	gui_one.draw(guiShader, model_matrix_location);
	
	glUniform3fv(colour_location, 1, vec3(0.0, 0.0, 1.0).v);
	gui_two.draw(guiShader, model_matrix_location);

	glUniform3fv(colour_location, 1, vec3(1.0, 1.0, 0.0).v);
	gui_three.draw(guiShader, model_matrix_location);

	glUniform3fv(colour_location, 1, vec3(1.0, 1.0, 1.0).v);
	gui_slash.draw(guiShader, model_matrix_location);
}

void drawObjects() {
	mat4 view = camera.getView();
	mat4 persp_proj = perspective(camera.getFOV(), (float)width / (float)height, 0.1f, 1000.0f);

	// draw regular objects
	shader.use();

	int view_mat_location = glGetUniformLocation(shader.getProgram(), "view");
	int proj_mat_location = glGetUniformLocation(shader.getProgram(), "projection");
	int matrix_location = glGetUniformLocation(shader.getProgram(), "model");
	int light_pos_location = glGetUniformLocation(shader.getProgram(), "lightPos");
	int view_pos_location = glGetUniformLocation(shader.getProgram(), "viewPos");

	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(light_pos_location, 1, GL_FALSE, vec3(5.0, 50.0, 5.0).v);
	glUniformMatrix4fv(view_pos_location, 1, GL_FALSE, camera.getView().m);

	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);

	// fog
	if (toggleFog) {
		int fog_sky_colour_location = glGetUniformLocation(shader.getProgram(), "skyColour");
		int should_toggle_fog_location = glGetUniformLocation(shader.getProgram(), "toggleFog");
		glUniform3fv(fog_sky_colour_location, 1, vec3(0.5f, 0.5f, 0.5f).v);
		glUniform1i(should_toggle_fog_location, toggleFog);
	}

	player.draw(shader, matrix_location);
	rock.draw(shader, matrix_location);
	tree.draw(shader, matrix_location);
	scene.draw(shader, matrix_location);
	light.draw(shader, matrix_location);

	for (int i = 0; i < cans.size(); i++)
		cans[i].draw(shader, matrix_location);

	for (int i = 0; i < rocks.size(); i++)
		rocks[i].draw(shader, matrix_location);
}

void drawSkybox() {
	glDepthMask(GL_FALSE);
	skyboxShader.use();

	int view_mat_location = glGetUniformLocation(skyboxShader.getProgram(), "view");
	int proj_mat_location = glGetUniformLocation(skyboxShader.getProgram(), "projection");
	int skybox_tex_location = glGetUniformLocation(skyboxShader.getProgram(), "skybox");
	mat4 persp_proj = perspective(camera.getFOV(), (float)width / (float)height, 0.1f, 1000.0f);

	// cube it up
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, camera.getView().m);

	skybox.draw();
	glDepthMask(GL_TRUE);
}

void drawWorld() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawObjects();
	drawSkybox();
}

void display() {
	drawWorld();
	drawGUI();

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
	Manager::getInstance().checkGameState(cans.size(), player.getHealth());

	gui_zero.reposition(player);
	gui_one.reposition(player);
	gui_two.reposition(player);
	gui_three.reposition(player);
	gui_slash.reposition(player);

	for (int i = 0; i < cans.size(); i++) {
		cans[i].update();
	}

	for (int i = 0; i < rocks.size(); i++) {
		rocks[i].update();
	}

	if (!DEV_MODE)
		ramblings.shouldPlay();

	// Draw the next frame
	glutPostRedisplay();
}

const int DISP_VERT = -250;

void init() {
	shader = Shader((GLchar*)File::getAbsoluteShaderPath("simpleShader.vert").c_str(),
		(GLchar*)File::getAbsoluteShaderPath("simpleShader.frag").c_str());

	skyboxShader = Shader((GLchar*)File::getAbsoluteShaderPath("skybox.vert").c_str(),
		(GLchar*)File::getAbsoluteShaderPath("skybox.frag").c_str());

	guiShader = Shader((GLchar*)File::getAbsoluteShaderPath("gui.vert").c_str(),
		(GLchar*)File::getAbsoluteShaderPath("gui.frag").c_str());

	player = Player(vec3(2.0f, -0.5f + DISP_VERT, 2.0f), MESH_PLAYER);
	tree = Scenery(vec3(-5.0f, -0.5f + DISP_VERT, 5.0f), MESH_TREE);
	scene = Scene(vec3(0.0f, -1.5f + DISP_VERT, 0.0f), MESH_GROUND, 10.0f);
	light = Light(vec3(0.0f, 0.0f + DISP_VERT, 0.0f), MESH_BOX);
	rock = Scenery(vec3(5.0f, -0.25f + DISP_VERT, 5.0f), MESH_ROCK);

	gui_zero = Number(player, vec3(-2.0, 4.5, 0.0), MESH_ZERO);
	gui_one = Number(player, vec3(-1.0, 4.5, 0.0), MESH_ONE);
	gui_two = Number(player, vec3(0.0, 4.5, 0.0), MESH_TWO);
	gui_three = Number(player, vec3(1.0, 4.5, 0.0), MESH_THREE);
	gui_slash = Number(player, vec3(2.0, 4.5, 0.0), MESH_SLASH);

	for (int i = 0; i < SPAWN_CAN_SIZE; i++) {
		srand((unsigned)time(0));
		cans.push_back(Cans(vec3((float)((rand() % 100) - 50),
			-0.25f, (float)((rand() % 100) - 50)), MESH_CANS));
	}

	for (int i = 0; i < 10; i++) {
		srand((unsigned)time(0));
		rocks.push_back(Scenery(vec3((float)(rand() % 100),
			-0.25f, (float)(rand() % 100)), MESH_ROCK));
	}

	Camera camera = Camera(player);

	//if (!DEV_MODE)
	//	music.playAudio(Sound::music);

	skybox.init();
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {

	if (key == 27) {
		glutExit();
	}

	// remove can from vector
	if (key == 'e') {
		for (int i = 0; i < cans.size(); i++) {
			if (player.tolerance(cans.at(i))) {
				cans.erase(cans.begin() + i);
				player.incrementHealth();
				if (!DEV_MODE)
					sfx.playAudio(Sound::sfx, Sound::BURP_SND);
			}
		}
	}

	if (key == 'q') {
		ramblings.playAudio(Sound::rambling);
	}

	if (key == 'f') {
		toggleFog = !toggleFog;
		std::cout << toggleFog << std::endl;
	}

	if (key == 'z') {
		camera.adjustFOV(-1);
	}

	player.onKey(key, camera.getTheta());
}

void releaseKeypress(unsigned char key, int x, int y) {
	player.onKeyRelease(key, camera.getTheta());
}

void mouseRoll(int button, int dir, int x, int y) {
	camera.adjustFOV((float)dir);
}

void mouseMove(int x, int y) {
	float xoffset = x - last_x;
	float yoffset = last_y - y;

	last_x = (float)x;
	last_y = (float)y;

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