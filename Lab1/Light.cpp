#pragma once
#include "GLIncludes.h"
#include "Math\maths_funcs.h"
#include "GameObject.cpp"
#include "Camera.cpp"

class Light : public GameObject {
private:
	float DIST_CAMERA = 1.0f;

public:
	Light() {}
	Light(vec3 pos, std::string meshName, float scale_coeff = 1.0f) 
		: GameObject(pos, meshName, scale_coeff) {

	}
	
	~Light() {}

	mat4 getLocation() {
		mat4 location = identity_mat4();
		location = translate(location, getPos());
		return location;
	}

	void updateViaCam(Camera& camera) {
		mat4 _pos = identity_mat4();
		_pos = rotate_y_deg(_pos, camera.getYaw());
		_pos = rotate_x_deg(_pos, camera.getTheta());
		_pos = rotate_z_deg(_pos, camera.getPitch());
		_pos = translate(_pos, camera.getPos() + vec3(0.0f, -1.0f, 0.0f));
		setModelMat(_pos);
	}
};

