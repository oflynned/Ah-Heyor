#pragma once

#include "GameObject.cpp"
#include "Player.cpp"

class Number : public GameObject {
private:
	vec3 offset;
	float y_rot = 0.0f;

public:
	Number() {}
	Number(Player& player, vec3 offset, std::string meshName) : GameObject(player.getPos() + offset, meshName, 1.0f) {
		this->offset = offset;
	}

	void reposition(Player& player) {
		y_rot += 0.1f;

		mat4 local_model_mat = identity_mat4();
		local_model_mat = translate(local_model_mat, offset);
		local_model_mat = rotate_y_deg(local_model_mat, y_rot);
		local_model_mat = scale(local_model_mat, vec3(4.0f, 4.0f, 4.0f));

		mat4 global = player.getModelMat() * local_model_mat;
		update(global);
	}
};