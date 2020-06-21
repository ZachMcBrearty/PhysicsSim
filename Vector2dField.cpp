#include "Vector2dField.h"
#include<GL/glut.h>

constexpr auto timestep = 0.1;
constexpr auto PI = 3.14159265;

Vector2dField::Vector2dField(int x_, int y_, int width_, double spacing_, int wavespeed_, bool bound_) {
	x = x_;
	y = y_;
	width = width_;
	spacing = spacing_;
	wavespeed = wavespeed_;
	bound = bound_;

	for (int i = 0; i < width; i++) {
		pos.push_back(0);
		vel.push_back(0);
		acc.push_back(0);
	}
}

void Vector2dField::interact() {
	if (bound) {
		auto prev = pos.begin();
		auto current = next(pos.begin(), 1);
		auto next_ = next(pos.begin(), 2);
		auto acc_ = next(acc.begin(), 1);
		for (int i = 0; i < width - 2; i++) {
			double avg = (*prev + *current + *next_) / 3;
			double lap = (6 / pow(spacing, 2)) * (avg - *current);
			*acc_ = (wavespeed ^ 2) * lap;
			++prev; ++current; ++next_; ++acc_;
		}
	}
	else {
		auto prev = pos.begin();
		auto current = pos.begin();
		auto next_ = next(pos.begin(), 1);
		auto acc_ = acc.begin();
		
		double avg = (*current + *next_) / 2;
		double lap = (6 / pow(spacing, 2)) * (avg - *current);
		*acc_ = (wavespeed ^ 2) * lap;
		++current; ++next_; ++acc_;

		for (int i = 0; i < width - 2; i++) {
			avg = (*prev + *current + *next_) / 3;
			lap = (6 / pow( spacing,2)) * (avg - *current);
			*acc_ = (wavespeed ^ 2) * lap;
			++prev; ++current; ++next_; ++acc_;
		}

		avg = (*prev + *current) / 2;
		lap = (6 / pow(spacing, 2)) * (avg - *current);
		*acc_ = (wavespeed ^ 2) * lap;
	}
	auto p = pos.begin();
	auto v = vel.begin();
	auto a = acc.begin();
	for (int i = 0; i < width; i++) {
		*v += *a * timestep;
		*p += *v * timestep;
		++p; ++v; ++a;
	}
}

void Vector2dField::draw() {
	glBegin(GL_LINES);
		glColor3f(1, 1, 1);
		glLineWidth(2);
		
		auto cur = pos.begin();
		auto nex = next(pos.begin(), 1);

		for (int i = 0; i < width - 1; i++) {
			glVertex2f(x + i * spacing, y + *cur);
			glVertex2f(x + ((__int64)i + 1) * spacing, y + *nex);
			++cur; ++nex;
		}

	glEnd();

}

double Vector2dField::getSpacing()
{
	return spacing;
}

int Vector2dField::getWidth()
{
	return width;
}

void Vector2dField::setPos(int i, double val)
{
	*next(pos.begin(), i) = val;
}

void Vector2dField::addPos(int i, double val)
{
	*next(pos.begin(), i) += val;
}

void Vector2dField::standingWave(int nodes, int size) {
	double step = nodes * PI / width;
	auto p = pos.begin();
	for (int i = 0; i < width; i++) {
		*p += size * (double)sin(i * step);
		++p;
	}
}

void Vector2dField::standingWave(int nodes, int size, double phase) {
	double step = nodes * PI / width;
	auto p = pos.begin();
	for (int i = 0; i < width; i++) {
		*p += size * (double)sin(phase + i * step);
		++p;
	}
}

void Vector2dField::wave(int nodes, int x, int w, int size) {
	double step = nodes * PI / w;
	auto p = next(pos.begin(), x);
	for (int i = 0; i < w; i++) {
		*p += size * (double)sin(i * step);
		++p;
	}
}

void Vector2dField::wave(int nodes, int x, int w, int size, double phase) {
	double step = nodes * PI / w;
	auto p = next(pos.begin(), x);
	for (int i = 0; i < w; i++) {
		*p += size * (double)sin(phase + i * step);
		++p;
	}
}