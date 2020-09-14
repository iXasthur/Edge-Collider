#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <cmath>
#include <windows.h>
#include "ColorFlow/ColorFlow.h"

const unsigned int FPS = 120;
const unsigned int MOVEMENT_TIMER_ID = 1;
const unsigned int MOVEMENT_UPDATE_DELAY = 1000/FPS;
const POINTFLOAT MOVEMENT_PER_FRAME = POINTFLOAT {200.0f/FPS, 100.0f/FPS};

const SIZE MIN_WINDOW_SIZE = SIZE {200, 150};
const SIZE FIRST_WINDOW_SIZE = SIZE {400, 300};
const SIZE MOVING_RECT_SIZE = SIZE {50, 50};

const COLORREF BACKGROUND_COLOR = RGB(26, 26, 26);

static COLORREF movingRectColor = RGB(241, 196, 15);
static POINTFLOAT movingRectPosition = POINTFLOAT {25.0f, 25.0f};
static POINTFLOAT movementDirectionModifier = POINTFLOAT {1.0f, 1.0f};

static ColorFlow movingRectColorFlow = ColorFlow(movingRectColor);


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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH backgroundBrush = CreateSolidBrush(BACKGROUND_COLOR);
            FillRect(hdc, &ps.rcPaint, backgroundBrush);

            SelectObject(hdc, GetStockObject(DC_PEN));
            SelectObject(hdc, GetStockObject(DC_BRUSH));

            COLORREF rectColor = movingRectColor;
            SetDCPenColor(hdc, rectColor);
            SetDCBrushColor(hdc, rectColor);

            RECT movingRect;
            movingRect.left = 0 + movingRectPosition.x;
            movingRect.top = 0 + movingRectPosition.y;
            movingRect.right = MOVING_RECT_SIZE.cx + movingRectPosition.x;
            movingRect.bottom = MOVING_RECT_SIZE.cy + movingRectPosition.y;

            Rectangle(hdc, movingRect.left, movingRect.top, movingRect.right, movingRect.bottom);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_GETMINMAXINFO: {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
            lpMMI->ptMinTrackSize.x = MIN_WINDOW_SIZE.cx;
            lpMMI->ptMinTrackSize.y = MIN_WINDOW_SIZE.cy;
            break;
        }
        case WM_TIMER: {
            movingRectPosition.x = movingRectPosition.x + MOVEMENT_PER_FRAME.x * movementDirectionModifier.x;
            movingRectPosition.y = movingRectPosition.y + MOVEMENT_PER_FRAME.y * movementDirectionModifier.y;

            movingRectColor = movingRectColorFlow.getNextColor();

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            if (movingRectPosition.x + MOVING_RECT_SIZE.cx > clientRect.right) {
                movingRectPosition.x = clientRect.right - MOVING_RECT_SIZE.cx;
                if (movementDirectionModifier.x > 0.0f) {
                    movementDirectionModifier.x = -1.0f;
                }
            } else if (movingRectPosition.x < clientRect.left) {
                movingRectPosition.x = clientRect.left;
                if (movementDirectionModifier.x < 0.0f) {
                    movementDirectionModifier.x = 1.0f;
                }
            }
            if (movingRectPosition.y + MOVING_RECT_SIZE.cy > clientRect.bottom) {
                movingRectPosition.y = clientRect.bottom - MOVING_RECT_SIZE.cy;
                if (movementDirectionModifier.y > 0.0f) {
                    movementDirectionModifier.y = -1.0f;
                }
            } else if (movingRectPosition.y < clientRect.top) {
                movingRectPosition.y = clientRect.top;
                if (movementDirectionModifier.y < 0.0f) {
                    movementDirectionModifier.y = 1.0f;
                }
            }

            InvalidateRect(hwnd, nullptr, false);
            break;
        }
        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return 0;
}