// 2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "2.h"
#include "Figure.h"
#include <cmath>
#include <CommCtrl.h>
#include <typeinfo>

#pragma warning(disable:4996)
#pragma comment(lib, "ComCtl32.Lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
const int PI = 3.1415;
#define STATUS_BAR_SIZE 18
const int ID_TIMER1 = 1;
const int ID_TIMER2 = 2;
int maxXCoord;
int maxYCoord;
int move = 1;
int changeStyle = 1;
int br = 0;
HWND hStatus;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



const wchar_t* getWC(const char* c) {
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY2);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	SetTimer(hWnd, ID_TIMER1, 2, NULL);
	SetTimer(hWnd, ID_TIMER2, 2000, NULL);


	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE, L"", hWnd, 4000);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static int x = 50, y = 50, size = 60, len = 5, dx = 5, dy = 5;
	static Figure* figure = new Circle(hWnd, size);


	switch (message) {


	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_BR:
			br = (br == 1) ? 0 : 1;
			break;
		case IDM_STOP1:
			move = (move == 1) ? 0 : 1;
			break;

		case IDM_STOP2:
			changeStyle = (changeStyle == 1) ? 0 : 1;
			break;

		case IDM_CIRCLE:
			delete figure;
			figure = new Circle(hWnd, size);

			break;

		case IDM_TRIANGLE:
			delete figure;
			figure = new Triangle(hWnd, size);

			break;

		case IDM_RHOMB:
			delete figure;
			figure = new Rhomb(hWnd, size);
			break;

		case IDM_PENTAGON:
			delete figure;
			figure = new Pentagon(hWnd, size);

			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
				   break;

	case WM_TIMER: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case ID_TIMER1: {
			
				RECT rect = { 0 };
				GetClientRect(hWnd, &rect);
				maxXCoord = rect.right - rect.left ;
				maxYCoord = rect.bottom - rect.top - STATUS_BAR_SIZE;



				if (br) {
					int rnd = (rand() * 31) % 50;
					if (rnd > 45) {
						dx = rand() % 2 * dx - dx + 4 * ((rand() % 2) - 1);
						dy = rand() % 2 * dy - dy + 4 * ((rand() % 2) - 1);
					}
				}
				

				if (changeStyle) {
					int rnd = (rand() * 31) % 50;
					if (rnd > 30) {
						figure->setRandomBrush();
						figure->setRandomPen();
					}
				}

				if (move) {

				int cSize = 0;
				if (figure->points.size() == 1) {
					cSize = size;
				}



				for (int i = 0; i < figure->points.size(); ++i) {

					if (figure->points[i].x > maxXCoord - cSize) {
						dx = -dx;
						x += maxXCoord - figure->points[i].x - cSize;
						break;
					}

					if (figure->points[i].y > maxYCoord - cSize) {
						dy = -dy;
						y += maxYCoord - figure->points[i].y - cSize;
						break;
					}

					if (figure->points[i].x < 0) {
						dx = -dx;
						x += -figure->points[i].x;
						break;
					}

					if (figure->points[i].y < 0) {
						dy = -dy;
						y += -figure->points[i].y;
						break;
					}

				}

				x += dx;
				y += dy;
				figure->setXY(x, y);


			}

			InvalidateRect(hWnd, NULL, TRUE);
			break;

		}





		case ID_TIMER2: {
			LONG_PTR pParts[5];

			pParts[0] = 200;
			pParts[1] = 200 + 100;
			pParts[2] = 200 + 100 + 100;
			//pParts[3] = 200 + 100 + 100 + 100;
			//pParts[4] = 200 + 100 + 100 + 100 + 100;


			SendMessage(hStatus, SB_SETPARTS, 3, (LPARAM)&pParts);
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)getWC(figure->getHBColor()));
			SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)getWC(figure->getPenColor()));
			SendMessage(hStatus, SB_SETTEXT, 2, (LPARAM)getWC(figure->getPenStyle()));

			break;
		}
		}
	}



	case WM_PAINT: {


		RECT rect = { 0 };
		GetClientRect(hWnd, &rect);
		int maxXCoord = rect.right - rect.left;
		int maxYCoord = rect.bottom - rect.top - STATUS_BAR_SIZE;
		figure->setXY(x, y);




		figure->draw();
		// delete figure;
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
				 return 0;

	}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
