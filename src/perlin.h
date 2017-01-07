#pragma once

/*
Based on the implementation provided by github.com/sol-rpgo/Perlin_Noise
Which is based on the java implementation: https://mrl.nyu.edu/~perlin/noise

Minor alterations have been made to make the code more readable.
*/

#include <vector>

class PerlinNoise {
	std::vector<int> p;
public:
	PerlinNoise(unsigned int seed);

	void reseed(unsigned int seed);

	double noise(double x, double y, double z);
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};