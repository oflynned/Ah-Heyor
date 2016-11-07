#pragma once
#include "GLIncludes.h"

#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

class GameObject {
private:
	vec3 pos;
	float rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
	float pos_x = 0.0f, pos_y = 0.0f, pos_z = 0.0f;

	GLuint vao = 0;

	mat4 model;
	int point_count;

public:
	GameObject() {}

	GameObject(vec3 pos, int point_count, GLuint vao) {
		this->pos = pos;
		this->point_count = point_count;
		this->vao = vao;
	}

	virtual ~GameObject() {}

	mat4 getModel() { return this->model; }
	vec3 getPos() { return this->pos; }
	int getPointCount() { return this->point_count; }
	int getVao() { return this->vao;}

	void update() {
		model = identity_mat4();
		model = rotate_x_deg(model, -90);
		model = rotate_y_deg(model, rot_y);
		model = translate(model, pos);
	}

	void draw(int mat_loc) {
		glBindVertexArray(vao);
		glUniformMatrix4fv(mat_loc, 1, GL_FALSE, getModel().m);
		glDrawArrays(GL_TRIANGLES, 0, getPointCount());
	}

	void translateObject(vec3 pos) {
		this->pos.v[0] += pos.v[0];
		this->pos.v[1] += pos.v[1];
		this->pos.v[2] += pos.v[2];
	}

	void rotate(float rotation) {
		this->rot_y += rotation;
	}

	virtual void onKey(unsigned char key) = 0;
};