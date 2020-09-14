#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <windows.h>
#include "ColorFlow/ColorFlow.h"
#include "MovingObject/RainbowRect.h"


const unsigned int FPS = 120;
const unsigned int MOVEMENT_TIMER_ID = 1;
const unsigned int MOVEMENT_UPDATE_DELAY = 1000/FPS;
const POINTFLOAT MOVEMENT_PER_FRAME = POINTFLOAT {200.0f/FPS, 100.0f/FPS};

const SIZE MIN_WINDOW_SIZE = SIZE {200, 150};
const SIZE FIRST_WINDOW_SIZE = SIZE {400, 300};
const COLORREF BACKGROUND_COLOR = RGB(26, 26, 26);

const SIZE RAINBOW_RECT_SIZE = SIZE {40, 40};
const POINTFLOAT RAINBOW_RECT_DEFAULT_POSITION = POINTFLOAT {25.0f, 25.0f};
const COLORREF RAINBOW_RECT_DEFAULT_COLOR = RGB(241, 196, 15);


bool runRainbowRectAnimation = false;
RainbowRect rainbowRect = RainbowRect(ColorFlow(RAINBOW_RECT_DEFAULT_COLOR), RAINBOW_RECT_DEFAULT_POSITION);


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

    const wchar_t WINDOW_CLASS[] = L"MAIN_WINDOW_CLASS";
    const wchar_t WINDOW_TITLE[] = L"Edge collider ^_^";

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WINDOW_CLASS;
    wc.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);

    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowExW(
            0x0,  // Optional window styles.
            WINDOW_CLASS,   // Window class
            WINDOW_TITLE,   // Window text
            WS_OVERLAPPEDWINDOW,   // Window style
            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, FIRST_WINDOW_SIZE.cx, FIRST_WINDOW_SIZE.cy,
            nullptr,   // Parent window
            nullptr,   // Menu
            hInstance,  // Instance handle
            nullptr   // Additional application data
    );

    if (hwnd == nullptr) {
        MessageBoxW(nullptr, L"Error creating window!", L"Attention", MB_OK);
        return 0;
    }

    SetTimer(hwnd, MOVEMENT_TIMER_ID, MOVEMENT_UPDATE_DELAY, nullptr);
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

void drawRainbowRect(HDC hdc) {
    COLORREF initialDCPenColor = GetDCPenColor(hdc);
    COLORREF initialDCBrushColor = GetDCBrushColor(hdc);

    COLORREF rectColor = rainbowRect.colorFlow.getCurrentColor();
    SetDCPenColor(hdc, rectColor);
    SetDCBrushColor(hdc, rectColor);

    RECT movingRect;
    movingRect.left = rainbowRect.position.x;
    movingRect.top = rainbowRect.position.y;
    movingRect.right = RAINBOW_RECT_SIZE.cx + rainbowRect.position.x;
    movingRect.bottom = RAINBOW_RECT_SIZE.cy + rainbowRect.position.y;

    Rectangle(hdc, movingRect.left, movingRect.top, movingRect.right, movingRect.bottom);

    SetDCPenColor(hdc, initialDCPenColor);
    SetDCBrushColor(hdc, initialDCBrushColor);
}

void fixRainbowRectBorderPosition(HWND hwnd) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    if (rainbowRect.position.x + RAINBOW_RECT_SIZE.cx > clientRect.right) {
        rainbowRect.position.x = clientRect.right - RAINBOW_RECT_SIZE.cx;
        if (rainbowRect.directionModifier.x > 0.0f) {
            rainbowRect.directionModifier.x = -1.0f;
        }
    } else if (rainbowRect.position.x < clientRect.left) {
        rainbowRect.position.x = clientRect.left;
        if (rainbowRect.directionModifier.x < 0.0f) {
            rainbowRect.directionModifier.x = 1.0f;
        }
    }
    if (rainbowRect.position.y + RAINBOW_RECT_SIZE.cy > clientRect.bottom) {
        rainbowRect.position.y = clientRect.bottom - RAINBOW_RECT_SIZE.cy;
        if (rainbowRect.directionModifier.y > 0.0f) {
            rainbowRect.directionModifier.y = -1.0f;
        }
    } else if (rainbowRect.position.y < clientRect.top) {
        rainbowRect.position.y = clientRect.top;
        if (rainbowRect.directionModifier.y < 0.0f) {
            rainbowRect.directionModifier.y = 1.0f;
        }
    }
}

void updateRainbowRect(HWND hwnd) {
    rainbowRect.position.x = rainbowRect.position.x + MOVEMENT_PER_FRAME.x * rainbowRect.directionModifier.x;
    rainbowRect.position.y = rainbowRect.position.y + MOVEMENT_PER_FRAME.y * rainbowRect.directionModifier.y;

    rainbowRect.colorFlow.getNextColor(); // Generates new color

    fixRainbowRectBorderPosition(hwnd);
}

void drawBackground(HDC hdc, PAINTSTRUCT ps) {
    HBRUSH backgroundBrush = CreateSolidBrush(BACKGROUND_COLOR);
    FillRect(hdc, &ps.rcPaint, backgroundBrush);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            SelectObject(hdc, GetStockObject(DC_PEN));
            SelectObject(hdc, GetStockObject(DC_BRUSH));

            drawBackground(hdc, ps);
            if (!rainbowRect.isHidden) {
                drawRainbowRect(hdc);
            }


//            Gdiplus::Graphics graphics = Gdiplus::Graphics(hdc);

//            // Create an Image object.
//            Gdiplus::Image image(L"apple.png");
//            // Create a Pen object.
//            Gdiplus::Pen pen (Gdiplus::Color(255, 255, 0, 0), 2);
//            // Draw the original source image.
//            graphics.DrawImage(&image, 10, 10);
//            // Create a Rect object that specifies the destination of the image.
//            Gdiplus::Rect destRect(50, 50, 50, 50);
//            // Draw the rectangle that bounds the image.
//            graphics.DrawRectangle(&pen, destRect);
//            // Draw the image.
//            graphics.DrawImage(&image, destRect);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_TIMER: {
            if (runRainbowRectAnimation) {
                updateRainbowRect(hwnd);
            }

            InvalidateRect(hwnd, nullptr, false);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_F1: {
                    rainbowRect = RainbowRect(ColorFlow(RAINBOW_RECT_DEFAULT_COLOR), RAINBOW_RECT_DEFAULT_POSITION);
                    runRainbowRectAnimation = true;
                    break;
                }
                case 0x31: { // 1 key
                    if (!rainbowRect.isHidden && runRainbowRectAnimation) {
                        runRainbowRectAnimation = false;
                    } else {
                        rainbowRect.isHidden = false;
                        runRainbowRectAnimation = true;
                    }
                    break;
                }
                case VK_F2: {
                    rainbowRect.isHidden = true;
                    runRainbowRectAnimation = false;

                    break;
                }
                case 0x32: { // 2 key
                    rainbowRect.isHidden = true;
                    runRainbowRectAnimation = false;
                    break;
                }
            }
            break;
        }
        case WM_MOUSEWHEEL: {
            runRainbowRectAnimation = false;

            int fwKeys = GET_KEYSTATE_WPARAM(wParam);
            int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

            if (fwKeys == MK_SHIFT) {
                if (zDelta > 0) {
                    rainbowRect.position.x += RAINBOW_RECT_SIZE.cx / 4.0f;
                } else {
                    rainbowRect.position.x -= RAINBOW_RECT_SIZE.cx / 4.0f;
                }
            } else {
                if (zDelta > 0) {
                    rainbowRect.position.y -= RAINBOW_RECT_SIZE.cy / 4.0f;
                } else {
                    rainbowRect.position.y += RAINBOW_RECT_SIZE.cy / 4.0f;
                }
            }

            fixRainbowRectBorderPosition(hwnd);
            break;
        }
        case WM_GETMINMAXINFO: {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
            lpMMI->ptMinTrackSize.x = MIN_WINDOW_SIZE.cx;
            lpMMI->ptMinTrackSize.y = MIN_WINDOW_SIZE.cy;
            break;
        }
        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return 0;
}