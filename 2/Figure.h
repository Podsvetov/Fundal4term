#pragma once
#include "stdio.h"
#include <vector>


class Figure {

protected:

	HWND hwnd;
	int size;
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hb;
	HPEN hp;
	char hbColor[100];
	char hpColor[100];
	char penStyle[100];
	int x, y;
public:

	std::vector<POINT> points;

	Figure(HWND hWnd, int size) : hwnd{ hWnd }, size{ size } {

	}



	void setRandomBrush() {
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		sprintf_s(hbColor, "%d, %d, %d", r, g, b);
		hb = CreateSolidBrush(RGB(r, g, b));
	}


	char* getHBColor() {
		return hbColor;
	}

	char* getPenStyle() {
		return penStyle;
	}

	char* getPenColor() {
		return hpColor;
	}

	void setRandomPen() {

		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		sprintf_s(hpColor, "%d, %d, %d", r, g, b);
		int style = (rand() % 6) - 1;
		switch (style) {
		case 0:
			strcpy_s(penStyle, "SOLID");
			break;

		case 1:
			strcpy_s(penStyle, "DASH");
			break;

		case 2:
			strcpy_s(penStyle, "DOT");
			break;

		case 3:
			strcpy_s(penStyle, "DASHDOT");
			break;

		case 4:
			strcpy_s(penStyle, "DASHDOTDOT");
			break;

		case 5:
			strcpy_s(penStyle, "NULL");
			break;
		}
		hp = CreatePen(style, 1, RGB(r, g, b));
	}

	virtual void draw() = 0;

	

	void move(int dx, int dy) {
		for (int i = 0; i < points.size(); ++i) {
			
			points[i].x += dx;
			points[i].y += dy;
		}
		x += dx;
		y += dy;
	}

	void setXY(int x, int y) {
		this->x = x;
		this->y = y;
	}


};

class Circle : public Figure {
public:
	Circle(HWND hWnd, int size) : Figure(hWnd, size) {
	
	};

	void draw() {
		POINT p;
		p.x = x;
		p.y = y;
		points.clear();
		points.push_back(p);
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, hb);
		Ellipse(hdc, x, y, x + size, y + size);
		SelectObject(hdc, hp);
		Ellipse(hdc, x, y, x + size, y + size);
		EndPaint(hwnd, &ps);
	}
};


class Triangle : public Figure {
public:
	Triangle(HWND hWnd, int size) : Figure(hWnd, size) {};

	void draw() {
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, hb);
		points.clear();
		POINT p[] = { {x, y}, { x + size, y + size }, { x - size, y + size } };
		for (size_t i = 0; i < 3; i++)
		{
			points.push_back(p[i]);
		}
		Polygon(hdc, p, sizeof(p) / sizeof(p[0]));
		SelectObject(hdc, hp);
		Polygon(hdc, p, sizeof(p) / sizeof(p[0]));
		EndPaint(hwnd, &ps);
	}
};

class Rhomb : public Figure {
public:
	Rhomb(HWND hWnd, int size) : Figure(hWnd, size) {};

	void draw() {
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, hb);
		points.clear();
		POINT vertices[] = { {x, y}, {x + size, y + size}, {x , y + 2 * size}, {x - size, y + size} };
		for (size_t i = 0; i < 4; i++)
		{
			points.push_back(vertices[i]);
		}

		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
		SelectObject(hdc, hp);
		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
		EndPaint(hwnd, &ps);
	}
};

class Pentagon : public Figure {
public:
	Pentagon(HWND hWnd, int size) : Figure(hWnd, size) {};

	void draw() {
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, hb);
		points.clear();

		POINT vertices[] = { {x, y}, {x + size, y + size},
		{x + size , y + 3 * size},  {x , y + 2 * size}, {x - size, y + 2 * size} };

		for (size_t i = 0; i < 5; i++)
		{
			points.push_back(vertices[i]);
		}

		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
		SelectObject(hdc, hp);
		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
		EndPaint(hwnd, &ps);
	}
};