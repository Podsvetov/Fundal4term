#pragma once

#include <vector>
#include <Windows.h>

class Figure {

private:

	HWND hwnd;
	PAINTSTRUCT ps;
	HBRUSH constColor;
	HBRUSH hb;
	HPEN hp;
	

public:
	std::vector<POINT> points;

	Figure(HWND hWnd) : hwnd{ hWnd } {
		setRandomBrush();
	}

	COLORREF getRandomColor() {
		return RGB(rand() * 255, rand() * 255, rand() * 255);
	}
	COLORREF getColor(int a, int b, int c) {
		return RGB(a,b,c);
	}
	void setColor(int a, int b, int c) {
		hb = CreateSolidBrush(getColor(a,b,c));
		constColor = hb;
	}
	void setRandomBrush() {
		hb = CreateSolidBrush(getRandomColor());
		constColor = hb;
	}

	void setBrush(int R, int G, int B) {
		hb = CreateSolidBrush(RGB(R, G, B));
	}

	void retColor() {
		hb = constColor;
	}

	void addPoint(POINT p) {
		points.push_back(p);
	}

	void draw(HDC hdc) {
		

		

		SelectObject(hdc, hb);


		Polygon(hdc, points.data(), points.size());
		
		
	}

	void move(int x, int y) {
		for (int i = 0; i < points.size(); ++i) {
			points[i].x += x;
			points[i].y += y;
		}
	}

	void setAngles(int count) {
		int firstX = points[0].x;
		int firstY = points[0].y;
		points.clear();

		RECT rect = { 0 };
		GetClientRect(hwnd, &rect);
		int maxXCoord = rect.right - rect.left;
		int maxYCoord = rect.bottom - rect.top;


		for (size_t i = 1; i < count; i++)
		{
			POINT p;
			p.x = rand() % maxXCoord;
			p.y = rand() % maxYCoord;
			addPoint(p);
		}
	}

	




};


