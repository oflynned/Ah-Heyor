#pragma once
#include "GLIncludes.h"
#include "GameObject.cpp"

class Light : public GameObject {
private:
	bool isVisible = true;
public:
	Light() {}
	Light(vec3 pos, std::string meshName, float scale_coeff) : GameObject(pos, meshName, scale_coeff) {

	}
	
	~Light() {}

	void setVisibility(bool isVisible) { this->isVisible = isVisible; }
	bool getVisibility() { return this->isVisible; }

	void onKey(unsigned char key) override {}
};