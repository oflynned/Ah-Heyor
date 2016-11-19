#pragma once

#include <string>
#include <vector>

#include "Shader.cpp"
#include "GLIncludes.h"

#include "Math\maths_funcs.h"
#include "assimp\Importer.hpp"

class Mesh {
private:
	GLuint vao, vbo, ebo;

	static const int UV = 2;
	static const int TUPLE = 3;

	static const int POS_ID = 0;
	static const int NORM_ID = 1;
	static const int TEX_ID = 2;
	static const int TAN_ID = 3;

	void generateMesh() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		// Positions
		glEnableVertexAttribArray(POS_ID);
		glVertexAttribPointer(POS_ID, TUPLE, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		// Normals
		glEnableVertexAttribArray(NORM_ID);
		glVertexAttribPointer(NORM_ID, TUPLE, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, normal));

		// Texture Coords
		glEnableVertexAttribArray(TEX_ID);
		glVertexAttribPointer(TEX_ID, UV, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

		// Tangent
		glEnableVertexAttribArray(TAN_ID);
		glVertexAttribPointer(TAN_ID, TUPLE, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));

		glBindVertexArray(0);
	}

public:
	struct Vertex {
		vec3 position;
		vec3 normal;
		vec2 texCoords;
		vec3 tangent;
	};

	struct Texture {
		GLuint id;
		std::string type;
		aiString path;
	};

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		
		generateMesh();
	}

	void draw(Shader shader) {
		for (GLuint i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			GLuint material_location = glGetUniformLocation(shader.getProgram(), (textures[i].type + "1").c_str());
			glUniform1f(material_location, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	GLuint getVAO() { return vao; }
};