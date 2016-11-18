#pragma once
#include "GameObject.cpp"

class Cans : public GameObject {
private:
public:
	Cans() {}
	Cans(vec3 pos, std::string meshName, float scale_coeff = 1.0f) : GameObject(pos, meshName, scale_coeff) {

	}
	~Cans() {}
};