#pragma once
#include "framework.h"
class Object {
private:
	HWND h;
	
public:
	int x, y;
	Object(HWND hwnd, int x, int y) : h{ hwnd }, x{ x }, y{ y } {

	}

	HWND getHWND() {
		return h;
	}

	int getX() {
		return x;
	}

	int getY() {
		
		return y;	
		
	}

	

	int getWidth() {
		RECT rect = { 0 };
		GetWindowRect(h, &rect);
		return  rect.right - rect.left;
	}

	int getHight() {
		RECT rect = { 0 };
		GetWindowRect(h, &rect);
		return rect.bottom - rect.top;
	}

	bool operator == (Object r) {
		if (h == r.getHWND()) return true;
		return false;
	}

	bool operator != (Object r) {
		return !(r == *this);
	}

	
};