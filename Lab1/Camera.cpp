#pragma once

#include "GLIncludes.h"

class Camera {
private:
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 60.0f;
	const float SENSITIVITY = 0.25f;
	const float FOV = 40.0f;

	vec3 pos, front, up, worldUp, right;
	float yaw, pitch, speed, sensitivity, fov;

	void updateVectors() {
		vec3 front;
		front.v[0] = cos(radians(this->yaw)) * cos(radians(this->pitch));
		front.v[1] = sin(radians(this->pitch));
		front.v[2] = sin(radians(this->yaw)) * cos(radians(this->pitch));
		
		this->front = normalise(front);
		this->right = normalise(cross(this->front, this->worldUp));
		this->up = normalise(cross(this->right, this->front));
	}

public:
	Camera(vec3 pos = vec3(3.0f, 3.0f, 3.0f)) {
		this->pos = pos;
		this->worldUp = vec3(0.0f, 1.0f, 0.0f);
		this->right = vec3(0.0f, 0.0f, -1.0f);
		this->yaw = YAW;
		this->speed = SPEED;
		this->sensitivity = SENSITIVITY;
		this->fov = FOV;

		updateVectors();
	}
	~Camera() {}

	mat4 getView() { return look_at(pos, pos + front, up); }
	float getFOV() { return this->fov; }
	void adjustFOV(float diff) {
		if (fov < 20) fov = 20;
		else if (fov > 60) fov = 60;
		else fov -= diff;
	}

	void keypress(unsigned char key, float delta) {
		float velocity = speed * delta;
		switch (key) {
		case 'w':
			this->pos += this->front * velocity;
			break;
		case 's':
			this->pos -= this->front * velocity;
			break;
		case 'a':
			this->pos -= this->right * velocity;
			break;
		case 'd':
			this->pos += this->right * velocity;
			break;
		}

		//this->pos.v[1] = 0.0f;
		//this->front.v[1] = 0.0f;
	}

	void mouseMove(float x_offset, float y_offset, bool constrained = true) {
		x_offset *= this->sensitivity;
		y_offset *= this->sensitivity;

		this->yaw += x_offset;
		this->pitch += y_offset;

		if (constrained) {
			// prevent gimbal lock
			if (this->pitch > 45.0f) this->pitch = 45.0f;
			if (this->pitch < -45.0f) this->pitch = -45.0f;
		}

		updateVectors();
	}
};