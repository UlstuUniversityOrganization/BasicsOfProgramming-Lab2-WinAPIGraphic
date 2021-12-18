
#include "framework.h"
#include "BasicsOfProgramming-Lab2-WinAPIGraphic.h"

#define MAX_LOADSTRING 100


HINSTANCE hInst;                        
WCHAR szTitle[MAX_LOADSTRING];      
WCHAR szWindowClass[MAX_LOADSTRING];   


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


    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BASICSOFPROGRAMMINGLAB2WINAPIGRAPHIC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BASICSOFPROGRAMMINGLAB2WINAPIGRAPHIC));

    MSG msg;

    // Цикл основного сообщения:
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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BASICSOFPROGRAMMINGLAB2WINAPIGRAPHIC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BASICSOFPROGRAMMINGLAB2WINAPIGRAPHIC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void DrawMercedesLogo(HDC hdc, COLORREF color, HBRUSH backgroundBursh)
{
    HBRUSH brush = CreateSolidBrush(color);
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    SelectObject(hdc, brush);
    SelectObject(hdc, pen);

    POINT center = { 640, 360 };
    int bigRadius = 865 - center.x;
    int minRadius = 838 - center.x;

    Ellipse(hdc, center.x - bigRadius, center.y - bigRadius, center.x + bigRadius, center.y + bigRadius);
    SelectObject(hdc, backgroundBursh);
    Ellipse(hdc, center.x - minRadius, center.y - minRadius, center.x + minRadius, center.y + minRadius);
    SelectObject(hdc, brush);

    POINT vertices[] =
    {
        461.8f, 258.1f,
        471.4,  255,
        639.4,  322,
        807.1,  255.3,
        817.4,  258,
        816,    268.8,
        673.8,  380.7,
        648.2,  560.6,
        639.7,  568.4,
        631.3,  559.5,
        606,    381.3,
        462.4,  268.5,
        461.8,  258.1
    };


    Polygon(hdc, vertices, sizeof(vertices) / sizeof(POINT));


    DeleteObject(brush);
    DeleteObject(pen);
}

void DrawPyramid(HDC hdc, POINT origin, int blockSize, int baseBlocksCount)
{
    int y = 0;
    while (baseBlocksCount > 0)
    {
        for (int x = 0; x < baseBlocksCount; x++)
        {
            Rectangle(hdc, origin.x + blockSize * y + blockSize * x, origin.y - blockSize * (y + 1), origin.x + blockSize * (x + 1) + blockSize * y, origin.y - blockSize * y);
        }
        y++;
        baseBlocksCount -= 2;
    }

}

struct Color
{
    int r;
    int g;
    int b;
};

float Interpolate(float v1, float v2, float t)
{
    return v1 + (v2 - v1) * t;
}

Color InterpolateColor(Color color1, Color color2, float t)
{
    int r = Interpolate(color1.r, color2.r, t);
    int g = Interpolate(color1.g, color2.g, t);
    int b = Interpolate(color1.b, color2.b, t);
    //return RGB(r, g, b);
    //Color col = { r, g, b }
    return Color{r, g, b};
}

HBRUSH CreateSolidBrushColored(Color color)
{
    return CreateSolidBrush(RGB(color.r, color.g, color.b));
}

void RectangleColored(_In_ HDC hdc,Color color, _In_ int left, _In_ int top, _In_ int right, _In_ int bottom)
{
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(color.r, color.g, color.b));
    HBRUSH brush = CreateSolidBrushColored(color);
    SelectObject(hdc, brush);
    SelectObject(hdc, pen);
    Rectangle(hdc, left, top, right, bottom);
    DeleteObject(brush);
    DeleteObject(pen);
}
void DrawPyramidColored(HDC hdc, Color color, POINT origin, int blockSize, int baseBlocksCount)
{
    HPEN pen = CreatePen(PS_SOLID, 1,  RGB(color.r, color.g, color.b));
    HBRUSH brush = CreateSolidBrushColored(color);
    SelectObject(hdc, brush);
    SelectObject(hdc, pen);
    DrawPyramid(hdc, origin, blockSize, baseBlocksCount);
    DeleteObject(brush);
    DeleteObject(pen);
}



void DrawDesert(HDC hdc, RECT clientRect, Color backgroundColor)
{

    //Color backgroundColor = { 91, 130, 189 };
    //Color darkSandColor = { 213, 173, 120 };

    Color sandColor = { 233, 207, 171 };
    Color cactusColor = { 101, 132, 55 };

    Color darkSandColor = { InterpolateColor(sandColor, { 213, 173, 120 }, 0.4f) };

    HBRUSH brush = CreateSolidBrushColored(darkSandColor);

    RectangleColored(hdc, darkSandColor,  clientRect.left, clientRect.top + clientRect.bottom * 0.5f, clientRect.right, clientRect.bottom);


    DrawPyramidColored(hdc, InterpolateColor(sandColor, backgroundColor, 0.5f), POINT{ 720, 450 }, 25, 33);

    DrawPyramidColored(hdc, InterpolateColor(sandColor, backgroundColor, 0.25f), POINT{ -300, 450 }, 50, 15);
    DrawPyramidColored(hdc, InterpolateColor(sandColor, backgroundColor, 0.25f), POINT{ 900, 450 }, 50, 13);


    DrawPyramidColored(hdc, InterpolateColor(sandColor, backgroundColor, 0.1f), POINT{ 300, 500 }, 50, 15);
    DrawPyramidColored(hdc, sandColor, POINT{ -100, 500 }, 70, 10);
    DrawPyramidColored(hdc, sandColor, POINT{ 800, 600 }, 80, 5);


    //RectangleColored(hdc, cactusColor, 496, 117, 700, 619);
    RectangleColored(hdc, cactusColor, 496, 117, 700, 619);

    DrawPyramidColored(hdc, InterpolateColor(sandColor, darkSandColor, 0.3f), POINT{ 100, 650 }, 90, 5);
}

HBRUSH background;
Color backgroundColor = { 91, 130, 189 };
//Color backgroundColor = { 182, 222, 242 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        background = CreateSolidBrush(RGB(91, 130, 189));
 
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT clientRect{};
            GetClientRect(hWnd, &clientRect);
            SelectObject(hdc, background);
            //RectangleColored(hdc, Color{ 91, 130, 189 }, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
            //RectangleColored(hdc, Color{ 182, 222, 242 }, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
            RectangleColored(hdc, backgroundColor, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
            DrawDesert(hdc, clientRect, backgroundColor);
            //DrawMercedesLogo(hdc, RGB(255, 255, 255), background);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
    {
        DeleteObject(background);
        PostQuitMessage(0);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
