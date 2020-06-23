 // 4.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "4.h"
#include "Figure.h"
#include <Commctrl.h>
#include <commdlg.h>
#include <wingdi.h>
#include <string>
using namespace std;
#pragma warning(disable:4996)

#define MAX_LOADSTRING 100

#define RADIUS_FOR_CORNER_MOVE 10

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
bool drawByClick = 0;
Figure* figureUnderEdit = NULL;
std::vector<Figure*> figures;
HWND hListBox;
static HWND hEdt1, hEdt2, hEdt3, hEdt4;
const int ID_LIST = 9999;
Figure* figure = NULL;
bool isMove = 0;
bool isDeform = 0;
int oldX;
int oldY;
POINT* pointUnderEdit;
int listBoxCount = 0;
HWND hnd;
HPALETTE hpal;
static HCURSOR hcHand, hcArrow;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);

int StrToInt(char* s)
{
	int temp = 0; // число
	int i = 0;
	int sign = 0; // знак числа 0- положительное, 1 - отрицательное
	if (s[i] == '-')
	{
		sign = 1;
		i++;
	}
	while (s[i] >= 0x30 && s[i] <= 0x39)
	{
		temp = temp + (s[i] & 0x0F);
		temp = temp * 10;
		i++;
	}
	temp = temp / 10;
	if (sign == 1)
		temp = -temp;
	return(temp);
}

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
    LoadStringW(hInstance, IDC_MY4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY4));

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

    return (int) msg.wParam;
}

int isPointInsidePolygon(POINT* p, int Number, int x, int y)
{
    int i1, i2, n, N, S, S1, S2, S3, flag;
    N = Number;
    for (n = 0; n < N; n++)
    {
        flag = 0;
        i1 = n < N - 1 ? n + 1 : 0;
        while (flag == 0)
        {
            i2 = i1 + 1;
            if (i2 >= N)
                i2 = 0;
            if (i2 == (n < N - 1 ? n + 1 : 0))
                break;
            S = abs(p[i1].x * (p[i2].y - p[n].y) +
                p[i2].x * (p[n].y - p[i1].y) +
                p[n].x * (p[i1].y - p[i2].y));
            S1 = abs(p[i1].x * (p[i2].y - y) +
                p[i2].x * (y - p[i1].y) +
                x * (p[i1].y - p[i2].y));
            S2 = abs(p[n].x * (p[i2].y - y) +
                p[i2].x * (y - p[n].y) +
                x * (p[n].y - p[i2].y));
            S3 = abs(p[i1].x * (p[n].y - y) +
                p[n].x * (y - p[i1].y) +
                x * (p[i1].y - p[n].y));
            if (S == S1 + S2 + S3)
            {
                flag = 1;
                break;
            }
            i1 = i1 + 1;
            if (i1 >= N)
                i1 = 0;
        }
        if (flag == 0)
            break;
    }
    return flag;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY4);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   hnd = hWnd;
   HMENU h = GetMenu(hWnd);
   hcHand = LoadCursor(NULL, IDC_HAND); 
   hcArrow = LoadCursor(NULL, IDC_ARROW);
   int b = EnableMenuItem(h, IDM_SETTINGS, MF_DISABLED | MF_GRAYED);
   DrawMenuBar(hWnd);
   
   hListBox = CreateWindow(_T("listbox"), NULL,
       WS_CHILD | WS_VISIBLE | LBS_STANDARD |
       LBS_WANTKEYBOARDINPUT,
       30, 30, 200, 100,
       hWnd, (HMENU)ID_LIST, hInst, NULL);
   ShowWindow(hListBox, SW_HIDE);
   
  


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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	
    switch (message)
    {


    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_DRAW_BY_CLICK:
                drawByClick = drawByClick == 0 ? 1 : 0;
                break;

            case IDM_SETTINGS: {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
                
                break;
            }

            case IDM_RANDOM: {
                
                RECT rect = { 0 };
                GetClientRect(hWnd, &rect);
                int maxXCoord = rect.right - rect.left;
                int maxYCoord = rect.bottom - rect.top;
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                for (int j = 0; j < 3; ++j) {
                    Figure* f = new Figure(hWnd);
                    int rx = rand() % maxXCoord / 2 + maxXCoord / 10;
                    int ry = rand() % maxYCoord / 2 +  maxYCoord / 10;
                    
                    POINT p[] = { {rx,ry} , {rx + rx / 2, ry},{rx + rx / 2 , ry + ry / 2} ,
                    {rx, ry + ry / 2} };
                       

                    f->addPoint(POINT(p[0]));
                    f->addPoint(POINT(p[1]));
                    f->addPoint(POINT(p[2]));
                    f->addPoint(POINT(p[3]));
                    f->draw(hdc);
                   
                    figures.push_back(f);
                }
                EndPaint(hWnd, &ps);
                ShowWindow(hListBox, SW_SHOW);




                for (int i = listBoxCount; i < figures.size(); ++i) {
                    wchar_t num[sizeof(i)];
                    _swprintf(num, L"%d", i);
                    
                    SendMessageW(hListBox, LB_ADDSTRING, 0,
                        (WPARAM) num );
                    SendMessageW(hListBox, LB_SETITEMDATA, i, (WPARAM)i);
                    
                }
                InvalidateRect(hWnd, NULL, TRUE);
                listBoxCount = figures.size();

                HMENU h = GetMenu(hWnd);
                int  b = EnableMenuItem(h, IDM_DRAW_BY_CLICK, MF_DISABLED | MF_GRAYED);
                DrawMenuBar(hWnd);

                drawByClick = 0;




                figureUnderEdit = NULL;

               
                break;
            }

            case ID_LIST: {
                switch (LOWORD(wParam)){
                    case ID_LIST: {

                        switch (HIWORD(wParam)){

                            case LBN_SELCHANGE: {
                                int n = (int)SendMessage(hListBox,
                                    LB_GETCURSEL, 0, 0L);
                                if (n != LB_ERR) {
                                    InvalidateRect(hWnd, NULL, TRUE);
                                    PAINTSTRUCT ps;
                                    HDC hdc = BeginPaint(hWnd, &ps);
                                    figures[n]->draw(hdc);
                                    figure = figures[n];
                                    EndPaint(hWnd, &ps);

                                    HMENU h = GetMenu(hWnd);
                                    int b = EnableMenuItem(h, IDM_SETTINGS, MF_ENABLED | MF_DEFAULT);
                                    DrawMenuBar(hWnd);

                                }
                        
                            }
                        }
                    }
                }
                break;
            }

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_LBUTTONDOWN: {
        if (drawByClick) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            ScreenToClient(hWnd, &cursorPos);
            if (!figureUnderEdit) {
                Figure* f = new Figure(hWnd);
                figures.push_back(f);
                figureUnderEdit = f;
            }

            figureUnderEdit->addPoint(cursorPos);

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        } else if (figure) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            ScreenToClient(hWnd, &cursorPos);
            for (int i = 0; i < figure->points.size(); ++i) {
                double distance = sqrt(pow(cursorPos.x - figure->points[i].x, 2) +
                    pow(cursorPos.y - figure->points[i].y, 2));
                
                if (distance < RADIUS_FOR_CORNER_MOVE) {
                    pointUnderEdit = &figure->points[i];
                    isDeform = 1;
                    oldX = cursorPos.x;
                    oldY = cursorPos.y;
                    break;
                }

                   
               

            }

            if (!isDeform && isPointInsidePolygon(figure->points.data(), figure->points.size(), cursorPos.x, cursorPos.y)) {
            
            isMove = 1;
            oldX = cursorPos.x;
            oldY = cursorPos.y;
        } 
        }

        break;
    }

    case WM_MOUSEMOVE: {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(hWnd, &cursorPos);
        for (Figure* f : figures) {
            if (isPointInsidePolygon(f->points.data(), f->points.size(), cursorPos.x, cursorPos.y)) {
				SetCursor(hcHand);
            }
            else {
				SetCursor(hcArrow);
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);

        if (figure) {
           

            RECT rect = { 0 };
            GetClientRect(hWnd, &rect);
            int maxXCoord = rect.right - rect.left;
            int maxYCoord = rect.bottom - rect.top;

            for (int i = 0; i < figure->points.size(); ++i) {
               
                    if (figure->points[i].x > maxXCoord) {
                        figure->move(maxXCoord-figure->points[i].x , 0);
                    }

                    if (figure->points[i].y > maxYCoord) {
                        figure->move(0, maxYCoord - figure->points[i].y );
                    }

                    if (figure->points[i].x < 0) {
                        figure->move(-figure->points[i].x,
                            0);
                    }

                    if (figure->points[i].y < 0) {
                        figure->move(0,
                            -figure->points[i].y);
                    }
                    
            }
            
            
            if (figure && isMove) {   
                figure->move(cursorPos.x - oldX, cursorPos.y - oldY);
            }
            else if (isDeform) {
                pointUnderEdit->x += cursorPos.x - oldX;
                pointUnderEdit->y += cursorPos.y - oldY;  
            }
            oldX = cursorPos.x;
            oldY = cursorPos.y;
            InvalidateRect(hWnd, NULL, TRUE);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            figure->draw(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    }

    case WM_LBUTTONUP:
        isMove = 0;
        isDeform = 0;
        break;


    case WM_RBUTTONDOWN: {
        figureUnderEdit = NULL;
    }


    case WM_PAINT:
        {

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            for (Figure *f : figures) {
                f->draw(hdc);
            }
            EndPaint(hWnd, &ps);
        }
        break;
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

BOOL __stdcall DlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	
	
    HWND sliderBar1 = GetDlgItem(hDlg, IDC_SLIDER1);
    HWND sliderBar2 = GetDlgItem(hDlg, IDC_SLIDER2);
    HWND sliderBar3 = GetDlgItem(hDlg, IDC_SLIDER3);
    HWND sliderBar4 = GetDlgItem(hDlg, IDC_SLIDER4);

	HWND text1 = GetDlgItem(hDlg, IDC_EDIT1);
	HWND text2 = GetDlgItem(hDlg, IDC_EDIT2);
	HWND text3 = GetDlgItem(hDlg, IDC_EDIT3);
	HWND text4 = GetDlgItem(hDlg, IDC_EDIT4);
	
	wchar_t buf[256];
	wstring s;
    static int angles = 1;
    static int r = 0, g = 0, b = 0;
    switch (Msg)
    {
    case WM_INITDIALOG:
        SendMessage(sliderBar1, TBM_SETRANGEMIN, false, 1);   //минимум
        SendMessage(sliderBar1, TBM_SETRANGEMAX, false, 255); //максимум
        SendMessage(sliderBar1, TBM_SETTICFREQ, false, 1);   //шаг
        SendMessage(sliderBar2, TBM_SETRANGEMIN, false, 1);   //минимум
        SendMessage(sliderBar2, TBM_SETRANGEMAX, false, 255); //максимум
        SendMessage(sliderBar2, TBM_SETTICFREQ, false, 1);   //шаг
        SendMessage(sliderBar3, TBM_SETRANGEMIN, false, 1);   //минимум
        SendMessage(sliderBar3, TBM_SETRANGEMAX, false, 255); //максимум
        SendMessage(sliderBar3, TBM_SETTICFREQ, false, 1);   //шаг

        SendMessage(sliderBar4, TBM_SETRANGEMIN, false, 3);   //минимум
        SendMessage(sliderBar4, TBM_SETRANGEMAX, false, 7); //максимум
        SendMessage(sliderBar4, TBM_SETTICFREQ, false, 1);   //шаг
		
		
        angles = figure->points.size();
		
		
        return (INT_PTR) TRUE;

    case WM_HSCROLL:
    {
        if (sliderBar1 == (HWND)lParam)
        {
            int curNumb = SendMessage(sliderBar1, TBM_GETPOS, 0, 0); // Получаем текущее положение слайдера
            r = curNumb;
			
			
				swprintf_s(buf, L"%d", r);
			SendMessage(text1, WM_SETTEXT, 0, (LPARAM)buf);

        }

        if (sliderBar2 == (HWND)lParam)
        {
            int curNumb = SendMessage(sliderBar2, TBM_GETPOS, 0, 0); // Получаем текущее положение слайдера
            g = curNumb;
			swprintf_s(buf, L"%d", g);
			SendMessage(text2, WM_SETTEXT, 0, (LPARAM)buf);
        }

        if (sliderBar3 == (HWND)lParam)
        {
            int curNumb = SendMessage(sliderBar3, TBM_GETPOS, 0, 0); // Получаем текущее положение слайдера
            b = curNumb;
			swprintf_s(buf, L"%d", b);
			SendMessage(text3, WM_SETTEXT, 0, (LPARAM)buf);
        }

        if (sliderBar4 == (HWND)lParam)
        {
            int curNumb = SendMessage(sliderBar4, TBM_GETPOS, 0, 0); // Получаем текущее положение слайдера
            angles = curNumb;
			swprintf_s(buf, L"%d", angles);
			SendMessage(text4, WM_SETTEXT, 0, (LPARAM)buf);
        }

        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
             

        case IDC_BUTTON1: {
            figure->setColor(r, g, b);
            if (figure->points.size() != angles) {
                figure->setAngles(angles);
            }
			InvalidateRect(hnd, NULL, true);
            EndDialog(hDlg, LOWORD(wParam));
			
            return (INT_PTR)TRUE;
        }
        }
        break;
       
    }
    return static_cast<LRESULT>(false);
}
