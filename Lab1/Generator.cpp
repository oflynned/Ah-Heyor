#pragma once
#include <ctime>
#include <cstdlib>
#include <math.h>

#define AMPLITUDE 50.0f
#define OCTAVES 3
#define ROUGHNESS 0.3f
#define PI 3.14159265359
#define VERTEX_COUNT 128

class Generator {
private:
	int seed;
	int x_offset = 0, z_offset = 0;

	float getNoise(int x, int z) {
		//long currSeed = x * 49632 + z * 325176 + seed;
		//srand(currSeed);
		return rand() * 2.0f - 1.0f;
	}

	float getSmoothNoise(int x, int z) {
		float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + 
			getNoise(x - 1, z + 1) + getNoise(x + 1, z + 1)) / 16.0f;
		float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + 
			getNoise(x, z - 1) + getNoise(x, z + 1)) / 8.0f;
		float centre = getNoise(x, z) / 4.0f;

		return corners + sides + centre;
	}

	float interpolate(float a, float b, float blend) {
		double theta = blend * PI;
		float f = (float)(1.0f - cos(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}

	float getInterpolatedNoise(float x, float z) {
		int i_x = (int)x;
		int i_z = (int)z;

		float f_x = x - i_x;
		float f_z = z - i_z;

		float v1 = getSmoothNoise(i_x, i_z);
		float v2 = getSmoothNoise(i_x + 1, i_z);
		float v3 = getSmoothNoise(i_x, i_z + 1);
		float v4 = getSmoothNoise(i_x + 1, i_z + 1);

		float i1 = interpolate(v1, v2, f_x);
		float i2 = interpolate(v3, v4, f_x);

		return interpolate(i1, i1, f_z);
	}

public:
	Generator() {}
	~Generator() {}

	void init() {
		srand((unsigned)time(0));
		this->seed = rand() % 1000000;
	}

	float generateHeight(int x, int z) {
		float total = 0;
		float d = (float)pow(2, OCTAVES - 1);
		for (int i = 0; i < OCTAVES; i++) {
			float freq = (float)(pow(2, i) / d);
			float amplitude = (float)(pow(ROUGHNESS, i) * AMPLITUDE);
			total += getInterpolatedNoise((x + x_offset) * freq, (z + z_offset) * freq) * amplitude;
		}

		return total;
	}
};
