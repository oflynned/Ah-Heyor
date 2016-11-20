#pragma once

#include "GameObject.cpp"
#include "Player.cpp"

class Number : public GameObject {
private:
	vec3 offset;

public:
	Number() {}
	Number(Player& player, vec3 offset, std::string meshName) : GameObject(player.getPos() + offset, meshName, 1.0f) {
		this->offset = offset;
	}

	void reposition(Player& player) {
		vec3 newPos = player.getPos() + offset;
		update(newPos, true);
	}
};