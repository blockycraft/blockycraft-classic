#include "perlin.h"

#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

PerlinNoise::PerlinNoise(unsigned int seed) {
	p.resize(256);
	std::iota(p.begin(), p.end(), 0);
	std::default_random_engine engine(seed);
	std::shuffle(p.begin(), p.end(), engine);
	p.insert(p.end(), p.begin(), p.end());
}

void PerlinNoise::reseed(unsigned int seed) {
	p.resize(256);
	std::iota(p.begin(), p.end(), 0);
	std::default_random_engine engine(seed);
	std::shuffle(p.begin(), p.end(), engine);
	p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) {
	int cx = (int) floor(x) & 255;
	int cy = (int) floor(y) & 255;
	int cz = (int) floor(z) & 255;
	double rx = x - floor(x);
	double ry = y - floor(y);
	double rz = z - floor(z);
	double u = fade(rx);
	double v = fade(ry);
	double w = fade(rz);

	int A = p[cx] + cy;
	int AA = p[A] + cz;
	int AB = p[A + 1] + cz;
	int B = p[cx + 1] + cy;
	int BA = p[B] + cz;
	int BB = p[B + 1] + cz;

	double corners[8];
	corners[0] = grad(p[AA], rx, ry, rz);
	corners[1] = grad(p[BA], rx-1, ry, rz);
	corners[2] = grad(p[AB], rx, ry-1, rz);
	corners[3] = grad(p[BB], rx-1, ry-1, rz);
	corners[4] = grad(p[AA+1], rx, ry, rz-1);
	corners[5] = grad(p[BA+1], rx-1, ry, rz-1);
	corners[6] = grad(p[AB+1], rx, ry-1, rz-1);
	corners[7] = grad(p[BB+1], rx-1, ry-1, rz-1);

	double vu1 = lerp(v, lerp(u, corners[0], corners[1]), lerp(u, corners[2], corners[3]));
	double vu2 = lerp(v, lerp(u, corners[4], corners[5]), lerp(u, corners[6], corners[7]));

	return (lerp(w, vu1, vu2) + 1.0)/2.0;
}

double PerlinNoise::fade(double t) { 
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) { 
	return a + t * (b - a); 
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	double u, v;
	
	if (h < 8) {
		u = x;
	}  else {
		u = y;
	}

	if (h < 4) {
		v = y;	
	} else if ((h == 12 || h == 14)) {
		v = x;
	} else {
 		v = z;
	}

	if ((h & 1) != 0) {
		u = -u;
	}

	if ((h & 2) != 0) {
		v = -v;
	}

	return u + v;
}