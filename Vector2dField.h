#pragma once
#include<list>
using namespace std;

class Vector2dField {
public:
	Vector2dField(int x_, int y_, int width_, double spacing_, int wavespeed_, bool bound);
	void interact();
	void draw();
	double getSpacing();
	int getWidth();
	void setPos(int i, double val);
	void addPos(int i, double val);
	void standingWave(int nodes, int size);
	void standingWave(int nodes, int size, double phase);
	void wave(int nodes, int x, int width, int size);
	void wave(int nodes, int x, int w, int size, double phase);
private:
	int x, y, width, wavespeed;
	double spacing;
	bool bound;
	list<double> pos;
	list<double> vel;
	list<double> acc;
	//int lapacian(int i);
};