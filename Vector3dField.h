#pragma once
#include<list>
using namespace std;

class Vector3dField {
public:
	Vector3dField(int x_, int y_, int width_, int height_, double spacing_, int wavespeed_, bool bound_);
	void standingWave(int n, int m, int size);
	void wave(int n, int m, int x, int y, int wid, int hei, int size);
	void interact();
	void draw();
	void swapSmooth();
private:
	int x, y, width, height, wavespeed;
	double spacing;
	bool bound, smooth;
	list<list<double>> pos;
	list<list<double>> vel;
	list<list<double>> acc;
	//GLuint pointsVBO;
};
