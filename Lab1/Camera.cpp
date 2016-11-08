#pragma once

#include "GLIncludes.h"

class Camera {
private:
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 5.0f;
	const float SENSITIVITY = 0.25f;
	const float FOV = 45.0f;

	vec3 pos, front, up, worldUp, right;
	float yaw, pitch, speed, sensitivity, fov;

	void updateVectors() {
		vec3 front;
		front.v[0] = cos(radians(yaw)) * cos(radians(pitch));
		front.v[1] = sin(radians(pitch));
		front.v[2] = sin(radians(yaw)) * cos(radians(pitch));
		
		this->front = normalise(front);
		this->right = normalise(cross(front, worldUp));
		this->up = normalise(cross(right, front));
	}

public:
	Camera(vec3 pos = vec3(0.0f, 0.0f, 3.0f)) {
		this->pos = pos;
		this->up = vec3(0.0f, 1.0f, 0.0f);
		this->worldUp = vec3(0.0f, 1.0f, 0.0f);
		this->right = vec3(0.0f, 0.0f, -1.0f);
		this->yaw = YAW;
		this->speed = SPEED;
		this->speed = SPEED;
		this->sensitivity = SENSITIVITY;
		this->fov = FOV;

		updateVectors();
	}
	~Camera() {}

	mat4 getView() { return look_at(pos, pos + front, up); }
	float getFOV() { return this->fov; }
	void adjustFOV(float diff) {
		if (fov < 10) fov = 10;
		else if (fov > 120) fov = 120;
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
	}

	void mouseMove(float x_offset, float y_offset, bool constrained = true) {
		x_offset *= sensitivity;
		y_offset *= sensitivity;

		yaw += x_offset;
		pitch += y_offset;

		if (constrained) {
			// prevent gimball lock
			if (pitch > 89.0f) pitch = 89.0f;
			if (pitch < -89.0f) pitch = -89.0f;
		}

		updateVectors();
	}
};