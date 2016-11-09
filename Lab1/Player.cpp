#pragma once
#include "GameObject.cpp"

class Player : public GameObject {
private:
public:
	Player() {}
	Player(vec3 pos, std::string meshName, float scale_coeff = 0.2f) : GameObject(pos, meshName, scale_coeff) {
		
	}
	~Player() {}

	void onKey(unsigned char key) override {
		switch (key) {
		case 'w':
			translateObject(vec3(0.0f, 0.0f, 1.0f));
			break;
		case 's':
			translateObject(vec3(0.0f, 0.0f, -1.0f));
			break;
		case 'a':
			translateObject(vec3(1.0f, 0.0f, 0.0f));
			break;
		case 'd':
			translateObject(vec3(-1.0f, 0.0f, 0.0f));
			break;
		}
	}
};