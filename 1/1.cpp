// 1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "1.h"
#include <boost/date_time.hpp>
using namespace boost::local_time;
using namespace std;


#define MAX_LOADSTRING 100
#define ID_BUTTON 666
#define ID_TEXTBOX1 667
#define ID_TEXTBOX2 668
#define ID_TEXTBOX3 669

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND textBox1;
HWND textBox2;
HWND textBox3;
HWND button;
boost::posix_time::ptime date_time;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance); // прогер создаёт окно
BOOL                InitInstance(HINSTANCE, int); // прогер
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM); // система когда появляется событие 
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM); // система


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, //  вызывается системой как начальная точка входа для прикладной программы, о параметрах http://vsokovikov.narod.ru/New_MSDN_API/Window/fn_winmain.htm
                     _In_opt_ HINSTANCE hPrevInstance, 
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance); // прев инстанс в данном случае не делает ничего, а вообще эта штука нужна чтобы указывать на предыдущий экземпляр того же окна.
    UNREFERENCED_PARAMETER(lpCmdLine);  // указатель на командную строк

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1)); // дескриптор загруженной таблицы горячих клавиш, мейкин -- преобразует число в строку

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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW; //стиль окна
    wcex.lpfnWndProc    = WndProc; //Адрес функции обработки сообщений нашего окна
    wcex.cbClsExtra     = 0; //Дополнительный класс окна
    wcex.cbWndExtra     = 0; //Адрес функции рисования нашего окна,
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1)); // загрузка хендла иконки(стандартная или из ресурсов)
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW); // загрузка хендла курсора(стандартная или из ресурсов)
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1); // Цвет заднего фона окна
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1); //	Название меню нашего окна
    wcex.lpszClassName  = szWindowClass; //	Имя класса нашего окна
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); //хендл маленькой иконки(свернутое окно)

    return RegisterClassExW(&wcex); // Регистрируем класс нашего окна - API
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

   textBox1 = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, // класс, название, стиль
       200, 30, 120, 20, hWnd, (HMENU)ID_TEXTBOX1, NULL, NULL); //x старта, у старта, относительные размеры, родитель, хандл меню(что бы это не было), два пустых указателя

   textBox2 = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
       400, 30, 120, 20, hWnd, (HMENU)ID_TEXTBOX2, NULL, NULL);

   textBox3 = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
       600, 30, 120, 20, hWnd, (HMENU)ID_TEXTBOX3, NULL, NULL);

   button = CreateWindow(_T("button"), _T("Принять"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
       400, 200, 120, 20, hWnd, (HMENU)ID_BUTTON, NULL, NULL);

   

   

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow); //Показать окно, параметр определяет КАК оно показывается
   UpdateWindow(hWnd); //говорит функции винмейн что надо обновить

   return TRUE;
}

void fillTextBoxWithPtime(boost::posix_time::ptime time) {
    std::stringstream stream1;
    std::stringstream stream2;
    boost::posix_time::time_facet* facet1 = new boost::posix_time::time_facet();
    facet1->format("%d.%m.%Y");
    stream1.imbue(std::locale(std::locale::classic(), facet1));
    stream1 << time;
    string str1 = stream1.str();
    wstring ws1(str1.begin(), str1.end());

    boost::posix_time::time_facet* facet2 = new boost::posix_time::time_facet();
    facet2->format("%H:%M:%S:%f");
    stream2.imbue(std::locale(std::locale::classic(), facet2));
    stream2 << time;
    string str2 = stream2.str();

    wstring ws2(str2.begin(), str2.end());

   // const int len = GetWindowTextLength(textBox1) + 1;
    //char* text = new char[len];
    //GetWindowText(textBox1, text, len);
    //Append the text to box 2.
    SendMessage(textBox1, EM_SETSEL, -1, -1);
    SendMessage(textBox1, EM_REPLACESEL, 0, (LPARAM)str1.c_str());
    SetWindowText(textBox1, ws1.c_str());

    SendMessage(textBox2, EM_SETSEL, -1, -1);
    SendMessage(textBox2, EM_REPLACESEL, 0, (LPARAM)str2.c_str());
    SetWindowText(textBox2, ws2.c_str());
    //delete[] text;
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  // обрабатывает сообщения
{

    static int interval = 0;
    switch (message)
    {
    case WM_COMMAND: // обработчик событий меню
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_BUTTON: {
                try {
                    int len = GetWindowTextLength(textBox1) + 1;
                    wchar_t* text = new wchar_t[len];
                    GetWindowText(textBox1, &text[0], len);
                    wstring ws(text);

                    string str(ws.begin(), ws.end());
                    std::vector<std::string> results;

                    boost::split(results, text, [](char c) {return c == '.'; });
                    if (results.size() != 3) throw exception();

                    tm pt_tm;
                    pt_tm.tm_mday = atoi(results[0].c_str());
                    pt_tm.tm_mon = atoi(results[1].c_str()) - 1;
                    pt_tm.tm_year = atoi(results[2].c_str()) - 1900;
                    pt_tm.tm_sec = 0;
                    pt_tm.tm_min = 0;
                    pt_tm.tm_hour = 0;
                    pt_tm.tm_wday = 0;
                    pt_tm.tm_yday = 0;
                    pt_tm.tm_isdst = 0;

                    delete text;

                    date_time = boost::posix_time::ptime_from_tm(pt_tm);

                    len = GetWindowTextLength(textBox2) + 1;
                    text = new wchar_t[len];
                    GetWindowText(textBox2, &text[0], len);
                    ws = wstring(text);


                    str = string(ws.begin(), ws.end());


                    boost::split(results, text, [](char c) {return c == ':'; });
                    if (results.size() != 4) throw exception();
                    delete text;
                    date_time += boost::posix_time::hours(atoi(results[0].c_str()));
                    date_time += boost::posix_time::minutes(atoi(results[1].c_str()));
                    date_time += boost::posix_time::seconds(atoi(results[2].c_str()));
                    date_time += boost::posix_time::milliseconds(atoi(results[3].c_str()));



                    len = GetWindowTextLength(textBox3) + 1;
                    text = new wchar_t[len];
                    GetWindowText(textBox3, &text[0], len);
                    ws = wstring(text);
                    delete text;

                    str = string(ws.begin(), ws.end());
                    interval = stoi(str);
                }
                catch (exception const& e) {
                    MessageBox(hWnd, TEXT("Операция выполнена успешно!"), TEXT("Ошибка во входных данных"), MB_OK | MB_ICONASTERISK);
                    break;
                }
               // date_time += boost::posix_time::milliseconds(interval);

                
                SetTimer(hWnd, 1, interval, NULL);
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            

            case IDM_TIME: {
                boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
                fillTextBoxWithPtime(timeLocal);
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_TIMER:
  
        date_time += boost::posix_time::milliseconds(interval);
        fillTextBoxWithPtime(date_time);
        break;
    
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
           
            TextOut(hdc, 200, 10, _T("Дата"), 5);
            TextOut(hdc, 400, 10, _T("Время"), 6);
            TextOut(hdc, 600, 10, _T("Интервал"), 9);
            
            
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
