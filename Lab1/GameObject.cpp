#pragma once
#include "GLIncludes.h"
#include "Model.cpp"
#include "Shader.cpp"
#include "File.cpp"

class GameObject {
protected:
	vec3 pos;
	vec3 scale_tuple = vec3(1.0f, 1.0f, 1.0f);

	float rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;

	mat4 modelMat;
	Model model;

public:
	GameObject() {}
	GameObject(vec3 pos, std::string meshName, float scale_coeff) {
		this->pos = pos;
		this->model = Model((GLchar*)File::getAbsoluteModelPath(meshName).c_str());
		this->scale_tuple = vec3(scale_coeff, scale_coeff, scale_coeff);
	}

	~GameObject() {}

	void setModelMat(mat4 modelMat) { this->modelMat = modelMat; }
	mat4 getModelMat() { return this->modelMat; }
	void setPos(vec3 pos) { this->pos = pos; }
	vec3 getPos() { return this->pos; }

	virtual void update(mat4 modelMat) {
		this->modelMat = modelMat;
	}

	virtual void update(vec3 newPos, bool flip_y = false) {
		modelMat = identity_mat4();
		modelMat = rotate_x_deg(modelMat, rot_x);
		modelMat = flip_y ? rotate_y_deg(modelMat, -rot_y) : rotate_y_deg(modelMat, rot_y);
		modelMat = rotate_z_deg(modelMat, rot_z);
		modelMat = scale(modelMat, scale_tuple);
		modelMat = translate(modelMat, newPos);
	}

	virtual void update(float cameraRot = 0.0f, bool hasCamera = false, bool isFlip = false) {
		//global
		modelMat = identity_mat4();
		modelMat = isFlip ? rotate_x_deg(modelMat, -90.0f) : rotate_x_deg(modelMat, rot_x);
		modelMat = hasCamera ? rotate_y_deg(modelMat, cameraRot) : rotate_y_deg(modelMat, rot_y);
		modelMat = rotate_z_deg(modelMat, rot_z);
		modelMat = scale(modelMat, scale_tuple);
		modelMat = translate(modelMat, pos);
	}

	void draw(Shader& shader, int mat_loc) {
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
	
	void setYRot(float rot) { this->rot_y = rot; }

	float getXRot() { return this->rot_x; }
	float getYRot() { return this->rot_y; }
	float getZRot() { return this->rot_z; }

	virtual void onKey(unsigned char key) {}
	virtual void onKey(unsigned char key, float cameraRot) {}
	virtual void onKeyRelease(unsigned char key, float cameraRot) {}

	bool tolerance(GameObject gameObject) {
		return (getPos().v[0] < gameObject.getPos().v[0] + 2) &&
			(getPos().v[0] > gameObject.getPos().v[0] - 2) &&
			(getPos().v[2] < gameObject.getPos().v[2] + 2) &&
			(getPos().v[2] > gameObject.getPos().v[2] - 2);
	}
};