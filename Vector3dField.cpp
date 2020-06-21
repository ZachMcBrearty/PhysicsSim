#include "Vector3dField.h"
#include<GL/glut.h>
#include<array>
#include<math.h>
#include<iostream>

constexpr auto timestep = 0.1;
constexpr auto PI = 3.14159265;

array<double, 3> hsvtorgb(double h, double s, double v) {
	double r, g, b;
	double p, q, t, fract;
	if (h >= 360) {
		h = 0.;
	}
	else {
		h /= 60.;
	}
	fract = h - floor(h);
	p = v * (1. - s); // 0
	q = v * (1. - s * fract); // 1 - fract
	t = v * (1. - s * (1. - fract)); // fract
	if (h > 6.) {
		r = g = b = 0;
	}
	else if (h > 5.) {
		r = v; g = p; b = q;
	}
	else if (h > 4.) {
		r = t; g = p; b = v;
	}
	else if (h > 3.) {
		r = p; g = q; b = v;
	}
	else if (h > 2.) {
		r = p; g = v; b = t;
	}
	else if (h > 1.) {
		r = q; g = v; b = p;
	}
	else if (h > 0.) {
		r = v; g = t; b = p;
	}
	else {
		r = g = b = 0;
	}
	return array<double, 3>{r, g, b };
}


Vector3dField::Vector3dField(int x_, int y_, int width_, int height_, double spacing_, int wavespeed_, bool bound_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	spacing = spacing_;
	wavespeed = wavespeed_;
	bound = bound_;
	smooth = true;

	for (int i = 0; i < width; i++) {
		list<double> p;
		list<double> v;
		list<double> a;
		for (int j = 0; j < height; j++) {
			p.push_back(0);
			v.push_back(0);
			a.push_back(0);
		}
		pos.push_back(p);
		vel.push_back(v);
		acc.push_back(a);
	}
	//glGenBuffers(1, &pointsVBO);
}

void Vector3dField::swapSmooth() {
	smooth = not smooth;
}

void Vector3dField::interact()
{
	if (bound) {
		list<list<double>>::iterator prev1 = pos.begin();
		list<list<double>>::iterator curr1 = next(pos.begin(), 1);
		list<list<double>>::iterator next1 = next(pos.begin(), 2);

		list<list<double>>::iterator acc1 = next(acc.begin(), 1);

		for (int i = 0; i < width - 2; i++) {
			list<double>::iterator prevprev = (*prev1).begin();
			list<double>::iterator currprev = (*curr1).begin();
			list<double>::iterator nextprev = (*next1).begin();

			list<double>::iterator prevcurr = next((*prev1).begin(), 1);
			list<double>::iterator currcurr = next((*curr1).begin(), 1);
			list<double>::iterator nextcurr = next((*next1).begin(), 1);

			list<double>::iterator prevnext = next((*prev1).begin(), 2);
			list<double>::iterator currnext = next((*curr1).begin(), 2);
			list<double>::iterator nextnext = next((*next1).begin(), 2);

			list<double>::iterator acc2 = next((*acc1).begin(), 1);

			for (int j = 0; j < height - 2; j++) {
				double avg = (*prevprev + *currprev + *nextprev
					+ *prevcurr + *currcurr + *nextcurr
					+ *prevnext + *currnext + *nextnext) / 9;
				double lap = (6 / pow(spacing, 2)) * (avg - *currcurr);
				*acc2 = (wavespeed ^ 2) * lap;
				++prevprev; ++currprev; ++nextprev;
				++prevcurr; ++currcurr; ++nextcurr; 
				++prevnext; ++currnext; ++nextnext;
				++acc2;
			}
			++prev1; ++curr1; ++next1; ++acc1;
		}
	}
	else {
		cout << 0;
	}

	list<list<double>>::iterator p1 = pos.begin();
	list<list<double>>::iterator v1 = vel.begin();
	list<list<double>>::iterator a1 = acc.begin();
	for (int i = 0; i < width; i++) {
		list<double>::iterator p2 = (*p1).begin();
		list<double>::iterator v2 = (*v1).begin();
		list<double>::iterator a2 = (*a1).begin();
		for (int j = 0; j < height; j++) {
			*v2 += *a2 * timestep;
			*p2 += *v2 * timestep;
			++p2; ++v2; ++a2;
		}
		++p1; ++v1; ++a1;
	}
}

void Vector3dField::draw()
{
	glBegin(GL_POINTS);
	glLineWidth(2);

	//double scale = atan(1);
	double h;
	auto curx = pos.begin();
	for (int i = 0; i < width; i++) {
		auto cury = (*curx).begin();
		for (int j = 0; j < height; j++) {
			if (smooth) {
				h = 360 / (1 + exp(-(*cury) / 50));
			}
			else {
				h = 360 / (1 + exp(round(-(*cury) / 50)));
			}
			array<double, 3> rgb = hsvtorgb(h, 1, .5);
			glColor3f(rgb[0], rgb[1], rgb[2]);
			glVertex2d(x + i * spacing, y + j * spacing);
			++cury;
		}
		++curx;
	}
	glEnd();
}

void Vector3dField::standingWave(int n, int m, int size) {
	double stepX = n * PI / ((int)width-1);
	double stepY = m * PI / ((int)height-1);
	list<list<double>>::iterator p1 = pos.begin();
	for (int i = 0; i < width; i++) {
		list<double>::iterator p2 = (*p1).begin();
		for (int j = 0; j < height; j++) {
			if (i == 0 || j == 0 || i == width - 1 || j == height - 1) {
				++p2;
			}
			else {
				*p2 += size * (double)sin(i * stepX) * (double)sin(j * stepY);
				++p2;
			}
		}
		++p1; 
	}
}

void Vector3dField::wave(int n, int m, int x, int y, int wid, int hei, int size) {
	double stepX = n * PI / wid;
	double stepY = m * PI / hei;
	list<list<double>>::iterator p1 = next(pos.begin(), x);
	for (int i = 0; i < wid; i++) {
		list<double>::iterator p2 = next((*p1).begin(), y);
		for (int j = 0; j < hei; j++) {
			if (i == 0 || j == 0 || i == width - 1 || j == height - 1) {
				++p2;
			}
			else {
				*p2 += size * (double)sin(i * stepX) * (double)sin(j * stepY);
				++p2;
			}
		}
		++p1;
	}
}
