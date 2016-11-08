#pragma once
#include "GLIncludes.h"
#include "Model.cpp"
#include "Shader.cpp"
#include "File.cpp"

class GameObject {
private:
	vec3 pos;
	vec3 scale_tuple = vec3(1.0f, 1.0f, 1.0f);

	float rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
	float scale_coeff = 1.0f;

	mat4 modelMat;
	Model model;
	Shader shader;

public:
	GameObject(vec3 pos, Shader shader) {
		this->pos = pos;
		this->shader = shader;
	}

	virtual ~GameObject() {}

	mat4 getModelMat() { return this->modelMat; }
	vec3 getPos() { return this->pos; }

	void update() {
		scale_tuple = vec3(scale_coeff, scale_coeff, scale_coeff);

		modelMat = identity_mat4();
		modelMat = rotate_x_deg(modelMat, rot_x);
		modelMat = rotate_y_deg(modelMat, rot_y);
		modelMat = rotate_z_deg(modelMat, rot_z);
		modelMat = scale(modelMat, scale_tuple);
		modelMat = translate(modelMat, pos);
	}

	void draw(int mat_loc) {
		glUniformMatrix4fv(mat_loc, 1, GL_FALSE, modelMat.m);
		model.draw(shader);
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