#pragma once
#include "GameObject.cpp"
#include <thread>

#define S_SIZE 800

class Scene : public GameObject {
private:
	/*
	GLfloat terrain[VERTEX_COUNT][VERTEX_COUNT];

	void generateGround() {
		Generator generator;
		static const int count = VERTEX_COUNT * VERTEX_COUNT;

		float vertices[count * 3];
		float normals[count * 3];
		float textureCoords[count * 3];
		int indices[6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)];
		int vertexPointer = 0;

		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * S_SIZE;

				float height = getHeight(j, i, generator);
				vertices[vertexPointer * 3 + 1] = height;
				terrain[j][i] = height;
				
				vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * S_SIZE;
				vec3 normal = calculateNormal(j, i, generator);
				
				//x y z
				normals[vertexPointer * 3] = normal.v[0];
				normals[vertexPointer * 3 + 1] = normal.v[1];
				normals[vertexPointer * 3 + 2] = normal.v[2];

				textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
				textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);

				vertexPointer++;
			}
		}
	}

	float getHeight(int x, int z, Generator generator) {
		return generator.generateHeight(x, z);
	}

	vec3 calculateNormal(int x, int z, Generator generator) {
		float heightL = getHeight(x - 1, z, generator);
		float heightR = getHeight(x + 1, z, generator);
		float heightD = getHeight(x, z - 1, generator);
		float heightU = getHeight(x, z + 1, generator);

		return vec3(heightL - heightR, 1.0f, heightD - heightU);
	}*/
	
public:
	Scene() {}
	Scene(vec3 pos, std::string meshName, float scale_coeff = 1.0f) : GameObject(pos, meshName, scale_coeff) {
		//generateGround();
	}
	~Scene() {}

	GLfloat getMesh() {}
};