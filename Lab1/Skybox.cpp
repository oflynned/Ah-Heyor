#pragma once

#include <vector>

#include "GLIncludes.h"
#include "SOIL.h"

#include "File.cpp"

class Skybox {
private:
	std::vector <std::string> faces;
	GLuint texture, skyboxVAO, skyboxVBO;

	const GLfloat skyboxVertices[108] = {
		-200.0f,  200.0f, -200.0f,
		-200.0f, -200.0f, -200.0f,
		200.0f, -200.0f, -200.0f,
		200.0f, -200.0f, -200.0f,
		200.0f,  200.0f, -200.0f,
		-200.0f,  200.0f, -200.0f,

		-200.0f, -200.0f,  200.0f,
		-200.0f, -200.0f, -200.0f,
		-200.0f,  200.0f, -200.0f,
		-200.0f,  200.0f, -200.0f,
		-200.0f,  200.0f,  200.0f,
		-200.0f, -200.0f,  200.0f,

		200.0f, -200.0f, -200.0f,
		200.0f, -200.0f,  200.0f,
		200.0f,  200.0f,  200.0f,
		200.0f,  200.0f,  200.0f,
		200.0f,  200.0f, -200.0f,
		200.0f, -200.0f, -200.0f,

		-200.0f, -200.0f,  200.0f,
		-200.0f,  200.0f,  200.0f,
		200.0f,  200.0f,  200.0f,
		200.0f,  200.0f,  200.0f,
		200.0f, -200.0f,  200.0f,
		-200.0f, -200.0f,  200.0f,

		-200.0f,  200.0f, -200.0f,
		200.0f,  200.0f, -200.0f,
		200.0f,  200.0f,  200.0f,
		200.0f,  200.0f,  200.0f,
		-200.0f,  200.0f,  200.0f,
		-200.0f,  200.0f, -200.0f,

		-200.0f, -200.0f, -200.0f,
		-200.0f, -200.0f,  200.0f,
		200.0f, -200.0f, -200.0f,
		200.0f, -200.0f, -200.0f,
		-200.0f, -200.0f,  200.0f,
		200.0f, -200.0f,  200.0f
	};

	GLuint loadCubemap(std::vector<std::string> faces) {
		GLuint textureID;
		glGenTextures(1, &textureID);

		int width, height;
		unsigned char* image;

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (GLuint i = 0; i < faces.size(); i++) {
			image = SOIL_load_image((GLchar*) faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return textureID;
	}

public:
	Skybox() {}
	~Skybox() {}

	void init() {
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);

		faces.push_back(File::getAbsoluteTexturesPath("Skybox/right.bmp"));
		faces.push_back(File::getAbsoluteTexturesPath("Skybox/left.bmp"));
		faces.push_back(File::getAbsoluteTexturesPath("Skybox/top.bmp"));
		faces.push_back(File::getAbsoluteTexturesPath("Skybox/bottom.bmp"));
		faces.push_back(File::getAbsoluteTexturesPath("Skybox/back.bmp"));
		faces.push_back(File::getAbsoluteTexturesPath("Skybox/front.bmp"));

		texture = loadCubemap(faces);
	}

	void draw() {
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	GLuint getTexture() { return this->texture; }
	GLuint getVAO() { return this->skyboxVAO; }
};