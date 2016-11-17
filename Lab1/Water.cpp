#pragma once

#include "GameObject.cpp"

class Water {
private:
	GLfloat* plane;

public:
	Water() {}
	~Water() {}

	GLfloat* getPlane() { return this->plane; }
};