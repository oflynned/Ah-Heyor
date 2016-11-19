#pragma once

#include "GLIncludes.h"

class GUI {
private:
	vec2 position, scale;
	vec4 colour;

public:
	GUI() {}
	GUI(vec2 position, vec2 scale, vec4 colour) {
		this->position = position;
		this->scale = scale;
		this->colour = colour;
	}
	virtual ~GUI() {}

	vec2 getPosition() { return this->position; }
	vec2 getScale() { return this->scale; }
	vec4 getColour() { return this->colour; }

	void render() {

	}
};