#pragma once
#include "GameObject.cpp"
#include "GLIncludes.h"

class Light {
private:
	mat4 sourceLocation;

public:
	Light(vec3 sourceLocation) {
		this->sourceLocation = identity_mat4();
		this->sourceLocation.m[3] = sourceLocation.v[0];
		this->sourceLocation.m[7] = sourceLocation.v[1];
		this->sourceLocation.m[11] = sourceLocation.v[2];
	}
	
	~Light() {}

	mat4 getLocation() { return this->sourceLocation; }
};